import cv2
import os
import time
import torch
from collections import defaultdict

import config as app_config
import data_manager
from stream_utils import SmoothStream, get_youtube_stream_url

try:
    from ultralytics import YOLO
except Exception:
    YOLO = None

print(f"CUDA Available: {torch.cuda.is_available()}")

def _point_side(px, py, ax, ay, bx, by):
    return (bx - ax) * (py - ay) - (by - ay) * (px - ax)

def _crossed_segment(prev_center, curr_center, p1, p2):
    if prev_center is None:
        return False
    prev_side = _point_side(prev_center[0], prev_center[1], p1[0], p1[1], p2[0], p2[1])
    curr_side = _point_side(curr_center[0], curr_center[1], p1[0], p1[1], p2[0], p2[1])
    return prev_side == 0 or curr_side == 0 or (prev_side < 0 < curr_side) or (prev_side > 0 > curr_side)

def _fit_to_display(image, max_width, max_height):
    height, width = image.shape[:2]
    if width <= max_width and height <= max_height:
        return image, 1.0, 1.0
    scale = min(max_width / width, max_height / height)
    new_size = (max(1, int(width * scale)), max(1, int(height * scale)))
    return cv2.resize(image, new_size, interpolation=cv2.INTER_AREA), scale, scale

def main():
    if YOLO is None:
        raise RuntimeError("ultralytics YOLO nicht installiert.")

    print(f"[INFO] Lade YOLO Modell aus {app_config.MODEL_PATH}...")
    model = YOLO(app_config.MODEL_PATH)

    loc, line_points, url = data_manager.get_current_location_data()
    line_points = [tuple(map(int, p)) for p in line_points[:2]]

    vehicle_counts = defaultdict(int)
    counted_track_ids = set()
    previous_centers = {}
    fullscreen = False
    display_scale = 1.0
    display_offset_x = 0
    display_offset_y = 0

    def _display_to_frame(x, y):
        if display_scale <= 0:
            return x, y
        return int((x - display_offset_x) / display_scale), int((y - display_offset_y) / display_scale)

    def on_mouse(event, x, y, flags, param):
        nonlocal line_points
        if event == cv2.EVENT_LBUTTONDOWN:
            fx, fy = _display_to_frame(x, y)
            if len(line_points) < 2:
                line_points.append((fx, fy))
                if len(line_points) == 2:
                    data_manager.update_line_points(loc, line_points)
                    print(f"[INFO] Linie gespeichert für {loc}: {line_points}")
            else:
                line_points = [(fx, fy)]
                data_manager.update_line_points(loc, line_points)

    def switch_location(new_loc):
        nonlocal loc, line_points, stream, vehicle_counts, counted_track_ids, previous_centers
        all_data = data_manager.load_data()
        if new_loc not in all_data.get("locations", {}):
            print(f"[ERROR] Location {new_loc} nicht gefunden.")
            return

        loc = new_loc
        stream.stopped = True
        data_manager.update_current_location(loc)

        line = all_data["locations"][loc].get("line", [])
        line_points = [tuple(map(int, p)) for p in line[:2]]

        try:
            new_url = all_data["locations"][loc]["url"]
            video_url = get_youtube_stream_url(new_url)
            stream = SmoothStream(video_url).start()
        except Exception as e:
            print(f"[ERROR] Stream-Fehler: {e}")

        vehicle_counts = defaultdict(int)
        counted_track_ids = set()
        previous_centers = {}
        print(f"[INFO] Gewechselt zu: {loc}")

    video_url = get_youtube_stream_url(url)
    stream = SmoothStream(video_url).start()

    cv2.namedWindow(app_config.WINDOW_NAME, cv2.WINDOW_NORMAL)
    cv2.resizeWindow(app_config.WINDOW_NAME, app_config.DISPLAY_WIDTH, app_config.DISPLAY_HEIGHT)
    cv2.setMouseCallback(app_config.WINDOW_NAME, on_mouse)

    frame_duration = 1.0 / app_config.FPS_TARGET
    last_time = time.time()

    while True:
        current_time = time.time()
        location_keys = {ord('1'): "tokyo", ord('2'): "koh_samui", ord('3'): "bangkok", ord('4'): "california", ord('5'): "london"}

        if current_time - last_time < frame_duration:
            key = cv2.waitKey(1) & 0xFF
            if key == ord('q'): break
            if key in location_keys: switch_location(location_keys[key])
            continue

        frame = stream.get_frame()
        if frame is not None:
            if len(stream.q) > 280:
                for _ in range(5): stream.pop_frame()

            results = model.track(
                frame, persist=True, tracker=app_config.TRACKER_CONFIG,
                conf=app_config.DEFAULT_CONFIDENCE, classes=app_config.TRACKED_CLASS_IDS,
                device=0, half=True, verbose=False
            )

            result = results[0]
            boxes = result.boxes

            if boxes is not None and boxes.id is not None and len(line_points) == 2:
                p1, p2 = line_points
                for box, track_id, cls_id in zip(boxes.xyxy, boxes.id.tolist(), boxes.cls.tolist()):
                    track_id = int(track_id)
                    cls_id = int(cls_id)
                    if cls_id not in app_config.TRACKED_CLASS_IDS: continue

                    x1, y1, x2, y2 = box.tolist()
                    center = (int((x1 + x2) / 2), int((y1 + y2) / 2))
                    prev = previous_centers.get(track_id)
                    previous_centers[track_id] = center

                    if _crossed_segment(prev, center, p1, p2) and track_id not in counted_track_ids:
                        counted_track_ids.add(track_id)
                        name = app_config.LABELS.get(cls_id, f"class_{cls_id}")
                        vehicle_counts[name] += 1

            if result.boxes is not None:
                result.boxes.is_track = False
            annotated = result.plot(conf=False)
            if len(line_points) == 2:
                cv2.line(annotated, line_points[0], line_points[1], (0, 255, 255), 2)


            display_frame, sx, sy = _fit_to_display(annotated, app_config.DISPLAY_WIDTH, app_config.DISPLAY_HEIGHT)
            display_scale = sx
            cv2.imshow(app_config.WINDOW_NAME, display_frame)
            stream.pop_frame()
            last_time = current_time

        if (cv2.waitKey(1) & 0xFF) == ord('q'): break

    stream.stopped = True
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()