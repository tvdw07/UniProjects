import cv2
import os
import time
import torch
from collections import defaultdict
from pathlib import Path

from stream_utils import SmoothStream, get_youtube_stream_url
import config as app_config

try:
    from ultralytics import YOLO
except Exception:
    YOLO = None

print(torch.cuda.is_available())
print(torch.version.cuda)
print(torch.cuda.device_count())

CONFIG_PATH = Path(__file__).with_name("config.py")
WINDOW_NAME = "RTX 4070 - No Lag Mode"
DISPLAY_WIDTH = 1280
DISPLAY_HEIGHT = 720


def _reload_config_values():
    import importlib
    importlib.reload(app_config)
    loc = getattr(app_config, "CURRENT_LOCATION", "tokyo")
    locations = getattr(app_config, "LOCATIONS", {})
    if loc in locations:
        line_points = locations[loc].get("line", [])
    else:
        line_points = getattr(app_config, "COUNTING_LINE_POINTS", [])

    tracked_ids = list(getattr(app_config, "TRACKED_CLASS_IDS", [2, 3, 5, 7]))
    labels = dict(getattr(app_config, "LABELS", {}))
    return line_points, tracked_ids, labels, loc


def _persist_counting_line_points(points, loc=None):
    if loc is None:
        loc = getattr(app_config, "CURRENT_LOCATION", "tokyo")
    text = CONFIG_PATH.read_text(encoding="utf-8")
    new_lines = []

    import importlib
    import config as temp_config
    importlib.reload(temp_config)
    locations = temp_config.LOCATIONS
    if loc in locations:
        locations[loc]["line"] = points

    import pprint
    locations_str = "LOCATIONS = " + pprint.pformat(locations, width=120)

    inside_locations = False
    for line in text.splitlines():
        if line.startswith("LOCATIONS ="):
            inside_locations = True
            new_lines.append(locations_str)
        elif inside_locations:
            if line.startswith("CURRENT_LOCATION =") or (line and not line.startswith(" ") and "=" in line and "}" not in line and not line.strip().startswith("}")):
                # overshot
                if line.startswith("CURRENT_LOCATION ="):
                    inside_locations = False
                    new_lines.append(line)
        else:
            new_lines.append(line)

    CONFIG_PATH.write_text("\n".join(new_lines) + "\n", encoding="utf-8")


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
        raise RuntimeError("ultralytics YOLO not installed. Install with: python -m pip install ultralytics")

    print("[INFO] Loading YOLO model...")
    model_path = "yolov8x.engine" if os.path.exists("yolov8x.engine") else "yolov8x.pt"
    model = YOLO(model_path)
    print(f"[INFO] YOLO model loaded from {model_path}.")

    line_points, tracked_class_ids, labels, loc = _reload_config_values()
    if not line_points:
        line_points = []
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
                    _persist_counting_line_points(line_points, loc)
                    print(f"[INFO] Counting line set to {line_points} and saved to config.py for {loc}")
            else:
                line_points = [(fx, fy)]
                _persist_counting_line_points(line_points, loc)
                print(f"[INFO] Restarted line selection with first point {(fx, fy)} for {loc}")

    def switch_location(new_loc):
        nonlocal loc, line_points, stream, vehicle_counts, counted_track_ids, previous_centers
        import config as temp_config
        import importlib
        importlib.reload(temp_config)
        locations = getattr(temp_config, "LOCATIONS", {})
        if new_loc not in locations:
            print(f"[ERROR] Location {new_loc} not found.")
            return

        loc = new_loc
        stream.stopped = True

        # update config
        text = CONFIG_PATH.read_text(encoding="utf-8")
        new_lines = []
        for line in text.splitlines():
            if line.startswith("CURRENT_LOCATION ="):
                new_lines.append(f"CURRENT_LOCATION = '{loc}'")
            else:
                new_lines.append(line)
        CONFIG_PATH.write_text("\n".join(new_lines) + "\n", encoding="utf-8")

        # load new line
        line = locations[loc].get("line", [])
        line_points = [tuple(map(int, p)) for p in line[:2]]

        # start new stream
        try:
            url = locations[loc]["url"]
            video_url = get_youtube_stream_url(url)
            stream = SmoothStream(video_url).start()
        except Exception as e:
            print(f"[ERROR] Failed to start stream for {loc}: {e}")

        # reset counts
        vehicle_counts = defaultdict(int)
        counted_track_ids = set()
        previous_centers = {}
        print(f"[INFO] Switched to location: {loc}")

    url = app_config.LOCATIONS.get(loc, {}).get("url", app_config.YOUTUBE_URL)
    VIDEO_URL = get_youtube_stream_url(url)

    # Instanz starten
    stream = SmoothStream(VIDEO_URL).start()

    print("[INFO] Using ultralytics.track with ByteTrack (tracker config expected, e.g. 'bytetrack.yaml')")

    cv2.namedWindow(WINDOW_NAME, cv2.WINDOW_NORMAL)
    cv2.resizeWindow(WINDOW_NAME, DISPLAY_WIDTH, DISPLAY_HEIGHT)
    cv2.setMouseCallback(WINDOW_NAME, on_mouse)

    # FPS-Kontrolle
    fps_target = 30
    frame_duration = 1.0 / fps_target
    last_time = time.time()

    # -----------------------------
    # MAIN LOOP
    # -----------------------------
    while True:
        current_time = time.time()

        location_keys = {
            ord('1'): "tokyo",
            ord('2'): "koh_samui",
            ord('3'): "bangkok",
            ord('4'): "california",
            ord('5'): "london",
        }

        if current_time - last_time < frame_duration:
            key = cv2.waitKey(1) & 0xFF
            if key == ord('q'):
                break
            if key == ord('f'):
                fullscreen = not fullscreen
                mode = cv2.WINDOW_FULLSCREEN if fullscreen else cv2.WINDOW_NORMAL
                cv2.setWindowProperty(WINDOW_NAME, cv2.WND_PROP_FULLSCREEN, mode)
            if key == ord('r'):
                line_points = []
                app_config.COUNTING_LINE_POINTS = line_points
                _persist_counting_line_points(line_points, loc)
                print("[INFO] Counting line reset")
            if key in location_keys:
                switch_location(location_keys[key])
            continue

        frame = stream.get_frame() # Schau in die Vergangenheit (Puffer-Anfang)

        if frame is not None:
            if len(stream.q) > 280:
                for _ in range(5):
                    stream.pop_frame()

            results = model.track(
                frame,
                persist=True,
                tracker="bytetrack.yaml",
                conf=0.28,
                classes=tracked_class_ids,
                device=0,
                half=True,
                verbose=False
            )

            result = results[0]
            boxes = result.boxes

            if boxes is not None and boxes.id is not None and len(line_points) == 2:
                p1, p2 = line_points
                for box, track_id, cls_id in zip(boxes.xyxy, boxes.id.tolist(), boxes.cls.tolist()):
                    track_id = int(track_id)
                    cls_id = int(cls_id)
                    if cls_id not in tracked_class_ids:
                        continue

                    x1, y1, x2, y2 = box.tolist()
                    center = (int((x1 + x2) / 2), int((y1 + y2) / 2))
                    previous_center = previous_centers.get(track_id)
                    previous_centers[track_id] = center

                    if _crossed_segment(previous_center, center, p1, p2) and track_id not in counted_track_ids:
                        counted_track_ids.add(track_id)
                        vehicle_name = labels.get(cls_id, f"class_{cls_id}")
                        vehicle_counts[vehicle_name] += 1
                        print(f"[COUNT] {vehicle_name}: {vehicle_counts[vehicle_name]} (track_id={track_id})")

            annotated = result.plot()
            if len(line_points) == 1:
                cv2.circle(annotated, line_points[0], 5, (0, 255, 255), -1)
                cv2.putText(annotated, "Click second point", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2)
            elif len(line_points) == 2:
                cv2.line(annotated, line_points[0], line_points[1], (0, 255, 255), 2)
                cv2.circle(annotated, line_points[0], 5, (0, 255, 255), -1)
                cv2.circle(annotated, line_points[1], 5, (0, 255, 255), -1)
                cv2.putText(annotated, f"Line: {line_points[0]} -> {line_points[1]}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2)
            else:
                cv2.putText(annotated, "Click 2 points to set counting line", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2)

            overlay_y = 65
            cv2.putText(annotated, "Counts:", (10, overlay_y), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 255, 255), 2)
            overlay_y += 28
            total_count = sum(vehicle_counts.values())
            cv2.putText(annotated, f"total: {total_count}", (10, overlay_y), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
            overlay_y += 26
            for name in sorted(vehicle_counts.keys()):
                cv2.putText(annotated, f"{name}: {vehicle_counts[name]}", (10, overlay_y), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
                overlay_y += 26

            display_frame, sx, sy = _fit_to_display(annotated, DISPLAY_WIDTH, DISPLAY_HEIGHT)
            display_scale = sx
            display_offset_x = max(0, (display_frame.shape[1] - int(annotated.shape[1] * sx)) // 2)
            display_offset_y = max(0, (display_frame.shape[0] - int(annotated.shape[0] * sy)) // 2)
            cv2.imshow(WINDOW_NAME, display_frame)

            stream.pop_frame()
            last_time = current_time

        key = cv2.waitKey(1) & 0xFF
        if key == ord('q'):
            break
        if key == ord('f'):
            fullscreen = not fullscreen
            mode = cv2.WINDOW_FULLSCREEN if fullscreen else cv2.WINDOW_NORMAL
            cv2.setWindowProperty(WINDOW_NAME, cv2.WND_PROP_FULLSCREEN, mode)
        if key == ord('r'):
            line_points = []
            app_config.COUNTING_LINE_POINTS = line_points
            _persist_counting_line_points(line_points, loc)
            print("[INFO] Counting line reset")
        if key in location_keys:
            switch_location(location_keys[key])

    stream.stopped = True
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
