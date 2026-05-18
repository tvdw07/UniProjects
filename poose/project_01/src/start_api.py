import cv2
import os
import time
import threading
from collections import defaultdict
from fastapi import FastAPI, Response, HTTPException
from fastapi.responses import StreamingResponse
from fastapi.middleware.cors import CORSMiddleware
import uvicorn

import config as app_config
import data_manager
from stream_utils import SmoothStream, get_youtube_stream_url

try:
    from ultralytics import YOLO
except Exception:
    YOLO = None

# Counter state
counter_state = {
    "running": False,
    "vehicle_counts": defaultdict(int),
    "counted_track_ids": set(),
    "previous_centers": {},
    "line_points": [],
    "model": None,
    "stream": None,
    "current_frame": None,
    "lock": threading.Lock(),
    "history": [],
}


def _point_side(px, py, ax, ay, bx, by):
    return (bx - ax) * (py - ay) - (by - ay) * (px - ax)


def _crossed_segment(prev_center, curr_center, p1, p2):
    if prev_center is None:
        return False
    prev_side = _point_side(prev_center[0], prev_center[1], p1[0], p1[1], p2[0], p2[1])
    curr_side = _point_side(curr_center[0], curr_center[1], p1[0], p1[1], p2[0], p2[1])
    return prev_side == 0 or curr_side == 0 or (prev_side < 0 < curr_side) or (prev_side > 0 > curr_side)


def _trim_counter_history(now_ts=None, max_age_seconds=600):
    now_ts = time.time() if now_ts is None else now_ts
    cutoff = now_ts - max_age_seconds
    counter_state["history"] = [entry for entry in counter_state["history"] if entry["ts"] >= cutoff]


def _record_counter_history(total_value):
    now_ts = time.time()
    counter_state["history"].append({"ts": now_ts, "total": int(total_value)})
    _trim_counter_history(now_ts)


def _snapshot_counter_stats():
    with counter_state["lock"]:
        counts = dict(counter_state["vehicle_counts"])
        running = counter_state["running"]
        line_points = list(counter_state["line_points"])
        now_ts = time.time()
        _trim_counter_history(now_ts)
        history = list(counter_state["history"])

    total = sum(counts.values())
    if len(history) >= 2:
        first = history[0]
        last = history[-1]
        elapsed = max(last["ts"] - first["ts"], 1.0)
        delta_total = max(0, int(last["total"] - first["total"]))
        avg_per_second = delta_total / elapsed
    else:
        avg_per_second = 0.0

    return {
        "total": total,
        "counts": counts,
        "running": running,
        "line_points": line_points,
        "history": history,
        "window_seconds": 600,
        "avg_per_second": avg_per_second,
        "avg_per_minute": avg_per_second * 60.0,
        "expected_30s": avg_per_second * 30.0,
        "samples": len(history),
        "history_start": history[0]["ts"] if history else None,
        "history_end": history[-1]["ts"] if history else None,
        "current_location": counter_state.get("current_location", "tokyo")
    }


def _process_frame(frame):
    if counter_state["model"] is None or not counter_state["running"]:
        return frame

    try:
        results = counter_state["model"].track(
            frame,
            persist=True,
            tracker=app_config.TRACKER_CONFIG,
            conf=app_config.DEFAULT_CONFIDENCE,
            classes=app_config.TRACKED_CLASS_IDS,
            device=0,
            half=True,
            verbose=False
        )

        result = results[0]
        boxes = result.boxes
        line_points = counter_state["line_points"]

        if boxes is not None and boxes.id is not None and len(line_points) == 2:
            p1, p2 = line_points
            for box, track_id, cls_id in zip(boxes.xyxy, boxes.id.tolist(), boxes.cls.tolist()):
                track_id = int(track_id)
                cls_id = int(cls_id)
                if cls_id not in app_config.TRACKED_CLASS_IDS:
                    continue

                x1, y1, x2, y2 = box.tolist()
                center = (int((x1 + x2) / 2), int((y1 + y2) / 2))
                previous_center = counter_state["previous_centers"].get(track_id)
                counter_state["previous_centers"][track_id] = center

                if _crossed_segment(previous_center, center, p1, p2) and track_id not in counter_state[
                    "counted_track_ids"]:
                    counter_state["counted_track_ids"].add(track_id)
                    vehicle_name = app_config.LABELS.get(cls_id, f"class_{cls_id}")
                    counter_state["vehicle_counts"][vehicle_name] += 1
                    print(
                        f"[COUNT] {vehicle_name}: {counter_state['vehicle_counts'][vehicle_name]} (track_id={track_id})")

        annotated = result.plot()

        if len(line_points) == 1:
            cv2.circle(annotated, line_points[0], 5, (0, 255, 255), -1)
            cv2.putText(annotated, "Click second point", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2)
        elif len(line_points) == 2:
            cv2.line(annotated, line_points[0], line_points[1], (0, 255, 255), 2)
            cv2.circle(annotated, line_points[0], 5, (0, 255, 255), -1)
            cv2.circle(annotated, line_points[1], 5, (0, 255, 255), -1)
            cv2.putText(annotated, f"Line: {line_points[0]} -> {line_points[1]}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX,
                        0.8, (0, 255, 255), 2)

        overlay_y = 65
        cv2.putText(annotated, "Counts:", (10, overlay_y), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 255, 255), 2)
        overlay_y += 28
        total_count = sum(counter_state["vehicle_counts"].values())
        cv2.putText(annotated, f"total: {total_count}", (10, overlay_y), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255),
                    2)
        overlay_y += 26
        for name in sorted(counter_state["vehicle_counts"].keys()):
            cv2.putText(annotated, f"{name}: {counter_state['vehicle_counts'][name]}", (10, overlay_y),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
            overlay_y += 26

        return annotated
    except Exception as e:
        print(f"[ERROR] Error processing frame: {e}")
        return frame


def counter_worker():
    frame_duration = 1.0 / app_config.FPS_TARGET
    last_time = time.time()

    while True:
        time.sleep(0.01)
        if not counter_state["running"] or counter_state["stream"] is None:
            continue

        current_time = time.time()
        if current_time - last_time < frame_duration:
            continue

        frame = counter_state["stream"].get_frame()
        if frame is not None:
            if len(counter_state["stream"].q) > 280:
                for _ in range(5):
                    counter_state["stream"].pop_frame()

            with counter_state["lock"]:
                annotated = _process_frame(frame)
                counter_state["current_frame"] = annotated

            counter_state["stream"].pop_frame()
            last_time = current_time


def stats_worker():
    while True:
        try:
            with counter_state["lock"]:
                total = sum(counter_state["vehicle_counts"].values())
                _record_counter_history(total)
        except Exception as e:
            print(f"[STATS] Error recording counter history: {e}")
        time.sleep(1)


app = FastAPI(
    title="Vehicle Counter API",
    description="""
# Vehicle Counter API

This API controls a machine learning-based vehicle counting system tracking various streams.

## Features
- **Stream Control**: Start, stop and reset the video analysis.
- **Location Management**: Change the camera / stream location dynamically.
- **Live Data**: Query statistics of the current counting session.
- **Media**: Access live image previews and continuous MJPEG video streams.
""",
    version="1.0.0",
    docs_url="/docs",
    redoc_url=None
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.on_event("startup")
async def startup_event():
    if YOLO is None:
        raise RuntimeError("ultralytics YOLO not installed.")

    print(f"[INFO] Loading YOLO model from {app_config.MODEL_PATH}...")
    counter_state["model"] = YOLO(app_config.MODEL_PATH)

    loc, line_points, _ = data_manager.get_current_location_data()
    counter_state["line_points"] = [tuple(map(int, p)) for p in line_points[:2]]
    counter_state["current_location"] = loc

    threading.Thread(target=counter_worker, daemon=True).start()
    threading.Thread(target=stats_worker, daemon=True).start()


@app.post("/start")
async def start_counter():
    """Starts the vehicle counter stream. If the stream is not initialized, it sets it up and begins counting."""
    if counter_state["stream"] is None:
        try:
            _, _, url = data_manager.get_current_location_data()
            video_url = get_youtube_stream_url(url)
            counter_state["stream"] = SmoothStream(video_url).start()
            print("[INFO] Stream started")
        except Exception as e:
            raise HTTPException(status_code=500, detail=f"Failed to start stream: {str(e)}")

    counter_state["running"] = True
    return {"status": "running", "message": "Counter started"}


@app.post("/location/{loc_id}")
async def set_location(loc_id: str):
    """Sets a new location for the vehicle counter, updating the camera view and counting line based on location data."""
    all_data = data_manager.load_data()
    if loc_id not in all_data.get("locations", {}):
        raise HTTPException(status_code=404, detail="Location not found")

    with counter_state["lock"]:
        counter_state["running"] = False
        if counter_state["stream"] is not None:
            counter_state["stream"].stopped = True
            counter_state["stream"] = None

        counter_state["vehicle_counts"] = defaultdict(int)
        counter_state["counted_track_ids"] = set()
        counter_state["previous_centers"] = {}
        counter_state["current_location"] = loc_id

        data_manager.update_current_location(loc_id)

        line = all_data["locations"][loc_id].get("line", [])
        counter_state["line_points"] = [tuple(map(int, p)) for p in line[:2]]

    try:
        url = all_data["locations"][loc_id]["url"]
        video_url = get_youtube_stream_url(url)
        counter_state["stream"] = SmoothStream(video_url).start()
        counter_state["running"] = True
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Failed to start stream: {str(e)}")

    return {"status": "success", "location": loc_id}


@app.post("/stop")
async def stop_counter():
    """Stops the vehicle counter stream from processing frames."""
    counter_state["running"] = False
    return {"status": "stopped"}


@app.post("/reset")
async def reset_counter():
    """Resets the statistics and counts of tracked vehicles back to zero."""
    with counter_state["lock"]:
        counter_state["vehicle_counts"] = defaultdict(int)
        counter_state["counted_track_ids"] = set()
        counter_state["previous_centers"] = {}
    return {"status": "reset"}


@app.get("/get_counter")
async def get_counter():
    """Retrieves the current state and totals of the tracked vehicles."""
    return _snapshot_counter_stats()


@app.get("/counter_stats")
async def counter_stats():
    """Retrieves detailed statistics and history of the vehicle counting process."""
    return _snapshot_counter_stats()


@app.get("/stream_preview")
async def stream_preview():
    """Returns a single image captured from the current frame of the stream."""
    if counter_state["current_frame"] is None:
        raise HTTPException(status_code=503, detail="No frame available")

    with counter_state["lock"]:
        frame = counter_state["current_frame"].copy()

    success, buffer = cv2.imencode('.png', frame)
    if not success:
        raise HTTPException(status_code=500, detail="Encoding failed")
    return Response(content=buffer.tobytes(), media_type="image/png")


def generate_stream():
    while True:
        if counter_state["current_frame"] is None:
            time.sleep(0.05)
            continue

        with counter_state["lock"]:
            frame = counter_state["current_frame"].copy()

        success, buffer = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, 80])
        if success:
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + buffer.tobytes() + b'\r\n')
        time.sleep(0.033)


@app.get("/stream")
async def stream():
    """Provides a continuous MJPEG stream of the annotated video feed."""
    if counter_state["model"] is None:
        raise HTTPException(status_code=503, detail="Model not initialized")
    return StreamingResponse(generate_stream(), media_type="multipart/x-mixed-replace; boundary=frame")


@app.get("/health")
async def health():
    """Health check endpoint to verify the API is running and returns basic status."""
    return {"status": "healthy", "running": counter_state["running"]}


@app.get("/")
async def root():
    """Root endpoint for basic API information."""
    return {"message": "Vehicle Counter API", "docs": "/docs"}


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)