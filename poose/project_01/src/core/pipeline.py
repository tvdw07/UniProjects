import cv2
import time
import threading
import config as app_config
from stream_utils import SmoothStream, get_youtube_stream_url
from .state import CounterState
import data_manager
import torch

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
        self.device = "cuda:0" if torch.cuda.is_available() else "cpu"
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

        try:
            results = self.model.track(
                frame,
                persist=True,
                tracker=app_config.TRACKER_CONFIG,
                conf=app_config.DEFAULT_CONFIDENCE,
                classes=app_config.TRACKED_CLASS_IDS,
                device=self.device,
                half=(self.device != "cpu"),
                verbose=False
            )
        except Exception as e:
            print(f"[WARN] GPU failed, fallback to CPU: {e}")

            results = self.model.track(
                frame,
                persist=True,
                tracker=app_config.TRACKER_CONFIG,
                conf=app_config.DEFAULT_CONFIDENCE,
                classes=app_config.TRACKED_CLASS_IDS,
                device="cpu",
                half=False,
                verbose=False
            )
            self.device = "cpu"
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

        with self.state.lock:
            show_boxes = self.state.show_boxes

        if show_boxes:
            if result.boxes is not None:
                result.boxes.is_track = False
            annotated = result.plot(conf=False)
        else:
            annotated = frame.copy()

        if len(line_points) == 1:
            cv2.circle(annotated, line_points[0], 5, (0, 255, 255), -1)
        elif len(line_points) == 2:
            cv2.line(annotated, line_points[0], line_points[1], (0, 255, 255), 2)
            cv2.circle(annotated, line_points[0], 5, (0, 255, 255), -1)
            cv2.circle(annotated, line_points[1], 5, (0, 255, 255), -1)

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
