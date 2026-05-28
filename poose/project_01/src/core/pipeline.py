import cv2
import time
import threading
import config as app_config
from stream_utils import SmoothStream, get_youtube_stream_url
from .state import CounterState
import data_manager

try:
    from ultralytics import YOLO
except Exception:
    YOLO = None

def _point_side(px, py, ax, ay, bx, by):
    return (bx - ax) * (py - ay) - (by - ay) * (px - ax)

def _crossed_segment(prev_center, curr_center, p1, p2):
    if prev_center is None:
        return False
    prev_side = _point_side(prev_center[0], prev_center[1], p1[0], p1[1], p2[0], p2[1])
    curr_side = _point_side(curr_center[0], curr_center[1], p1[0], p1[1], p2[0], p2[1])
    return prev_side == 0 or curr_side == 0 or (prev_side < 0 < curr_side) or (prev_side > 0 > curr_side)

class VehiclePipeline:
    def __init__(self, state: CounterState):
        self.state = state
        self.model = YOLO(app_config.MODEL_PATH) if YOLO is not None else None
        self.stream = None
        self._counter_thread = None
        self._stats_thread = None

    def load_location(self, loc_id):
        all_data = data_manager.load_data()
        if loc_id not in all_data.get("locations", {}):
            return False

        self.stop_stream()
        self.state.reset()

        with self.state.lock:
            self.state.current_location = loc_id
            line = all_data["locations"][loc_id].get("line", [])
            self.state.line_points = [tuple(map(int, p)) for p in line[:2]]
            url = all_data["locations"][loc_id]["url"]

        data_manager.update_current_location(loc_id)
        video_url = get_youtube_stream_url(url)
        self.stream = SmoothStream(video_url).start()
        return True

    def start_stream(self, url=None):
        if url is None:
            _, _, url = data_manager.get_current_location_data()
        video_url = get_youtube_stream_url(url)
        self.stream = SmoothStream(video_url).start()

    def stop_stream(self):
        if self.stream is not None:
            self.stream.stopped = True
            self.stream = None

    def process_frame(self, frame):
        if self.model is None:
            return frame

        results = self.model.track(
            frame, persist=True, tracker=app_config.TRACKER_CONFIG,
            conf=app_config.DEFAULT_CONFIDENCE, classes=app_config.TRACKED_CLASS_IDS,
            device=0, half=True, verbose=False
        )
        result = results[0]
        boxes = result.boxes

        with self.state.lock:
            line_points = list(self.state.line_points)
            is_running = self.state.running

        counted_track_ids = set()

        if is_running and boxes is not None and boxes.id is not None and len(line_points) == 2:
            p1, p2 = line_points
            for box, track_id, cls_id in zip(boxes.xyxy, boxes.id.tolist(), boxes.cls.tolist()):
                track_id = int(track_id)
                cls_id = int(cls_id)
                if cls_id not in app_config.TRACKED_CLASS_IDS:
                    continue

                x1, y1, x2, y2 = box.tolist()
                center = (int((x1 + x2) / 2), int((y1 + y2) / 2))

                with self.state.lock:
                    prev = self.state.previous_centers.get(track_id)
                    self.state.previous_centers[track_id] = center

                    if _crossed_segment(prev, center, p1, p2) and track_id not in self.state.counted_track_ids:
                        self.state.counted_track_ids.add(track_id)
                        counted_track_ids.add(track_id)
                        name = app_config.LABELS.get(cls_id, f"class_{cls_id}")
                        self.state.vehicle_counts[name] += 1
                        print(f"[COUNT] {name}: {self.state.vehicle_counts[name]} (track_id={track_id})")

        annotated = result.plot()
        if len(line_points) == 1:
            cv2.circle(annotated, line_points[0], 5, (0, 255, 255), -1)
            cv2.putText(annotated, "Click second point", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2)
        elif len(line_points) == 2:
            cv2.line(annotated, line_points[0], line_points[1], (0, 255, 255), 2)
            cv2.circle(annotated, line_points[0], 5, (0, 255, 255), -1)
            cv2.circle(annotated, line_points[1], 5, (0, 255, 255), -1)
            cv2.putText(annotated, f"Line: {line_points[0]} -> {line_points[1]}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2)

        overlay_y = 65
        cv2.putText(annotated, "Counts:", (10, overlay_y), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 255, 255), 2)
        overlay_y += 28

        with self.state.lock:
            total_count = sum(self.state.vehicle_counts.values())
            counts_copy = dict(self.state.vehicle_counts)

        cv2.putText(annotated, f"total: {total_count}", (10, overlay_y), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
        overlay_y += 26
        for name in sorted(counts_copy.keys()):
            cv2.putText(annotated, f"{name}: {counts_copy[name]}", (10, overlay_y), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
            overlay_y += 26

        return annotated

    def run_counter_worker(self):
        frame_duration = 1.0 / app_config.FPS_TARGET
        last_time = time.time()
        while True:
            time.sleep(0.01)
            stream = self.stream
            if stream is None:
                continue

            current_time = time.time()
            if current_time - last_time < frame_duration:
                continue

            frame = stream.get_frame()
            if frame is not None:
                if len(stream.q) > 280:
                    for _ in range(5):
                        stream.pop_frame()

                annotated = self.process_frame(frame)
                with self.state.lock:
                    self.state.current_frame = annotated
                stream.pop_frame()
                last_time = current_time

    def run_stats_worker(self):
        while True:
            try:
                self.state.record_history()
            except Exception as e:
                pass
            time.sleep(1)

    def start_workers(self):
        if not self._counter_thread:
            self._counter_thread = threading.Thread(target=self.run_counter_worker, daemon=True)
            self._counter_thread.start()
        if not self._stats_thread:
            self._stats_thread = threading.Thread(target=self.run_stats_worker, daemon=True)
            self._stats_thread.start()
