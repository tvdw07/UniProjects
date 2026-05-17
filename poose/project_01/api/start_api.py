import cv2
import os
import time
import torch
import threading
import asyncio
from datetime import datetime
from collections import defaultdict
from pathlib import Path
from io import BytesIO
import base64

from fastapi import FastAPI, Response, HTTPException
from fastapi.responses import StreamingResponse, JSONResponse, FileResponse
from fastapi.middleware.cors import CORSMiddleware
import uvicorn

from stream_utils import SmoothStream, get_youtube_stream_url
import config as app_config

try:
    from ultralytics import YOLO
except Exception:
    YOLO = None

# =============================================
# GLOBAL STATE
# =============================================
CONFIG_PATH = Path(__file__).with_name("config.py")
WINDOW_NAME = "RTX 4070 - No Lag Mode"
DISPLAY_WIDTH = 1280
DISPLAY_HEIGHT = 720

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
    "tracked_class_ids": [],
    "labels": {},
    "lock": threading.Lock(),
    "history": [],
}


# =============================================
# HELPER FUNCTIONS (copied from main.py)
# =============================================

def _reload_config_values():
    line_points = getattr(app_config, "COUNTING_LINE_POINTS", None)
    tracked_ids = list(getattr(app_config, "TRACKED_CLASS_IDS", [2, 3, 5, 7]))
    labels = dict(getattr(app_config, "LABELS", {}))
    return line_points, tracked_ids, labels


def _persist_counting_line_points(points):
    text = CONFIG_PATH.read_text(encoding="utf-8")
    new_lines = []
    replaced = False
    serialized = f"COUNTING_LINE_POINTS = {repr(points)}"
    for line in text.splitlines():
        if line.startswith("COUNTING_LINE_POINTS =") or line.startswith("COUNTING_LINE_Y ="):
            if not replaced:
                new_lines.append(serialized)
                replaced = True
        else:
            new_lines.append(line)
    if not replaced:
        new_lines.append(serialized)
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


def _trim_counter_history(now_ts=None, max_age_seconds=600):
    now_ts = time.time() if now_ts is None else now_ts
    cutoff = now_ts - max_age_seconds
    counter_state["history"] = [entry for entry in counter_state["history"] if entry["ts"] >= cutoff]


def _record_counter_history(total_value):
    now_ts = time.time()
    counter_state["history"].append({"ts": now_ts, "total": int(total_value)})
    _trim_counter_history(now_ts)


def _total_at_timestamp(history, target_ts):
    if not history:
        return None
    eligible = [entry for entry in history if entry["ts"] <= target_ts]
    if eligible:
        return int(eligible[-1]["total"])
    return int(history[0]["total"])


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
    }


def _process_frame(frame):
    """Process frame with tracking and counting logic"""
    if counter_state["model"] is None or not counter_state["running"]:
        return frame

    try:
        results = counter_state["model"].track(
            frame,
            persist=True,
            tracker="bytetrack.yaml",
            conf=0.25,
            classes=counter_state["tracked_class_ids"],
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
                if cls_id not in counter_state["tracked_class_ids"]:
                    continue

                x1, y1, x2, y2 = box.tolist()
                center = (int((x1 + x2) / 2), int((y1 + y2) / 2))
                previous_center = counter_state["previous_centers"].get(track_id)
                counter_state["previous_centers"][track_id] = center

                if _crossed_segment(previous_center, center, p1, p2) and track_id not in counter_state["counted_track_ids"]:
                    counter_state["counted_track_ids"].add(track_id)
                    vehicle_name = counter_state["labels"].get(cls_id, f"class_{cls_id}")
                    counter_state["vehicle_counts"][vehicle_name] += 1
                    print(f"[COUNT] {vehicle_name}: {counter_state['vehicle_counts'][vehicle_name]} (track_id={track_id})")

        annotated = result.plot()

        # Draw counting line and info
        line_points = counter_state["line_points"]
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
        total_count = sum(counter_state["vehicle_counts"].values())
        cv2.putText(annotated, f"total: {total_count}", (10, overlay_y), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
        overlay_y += 26
        for name in sorted(counter_state["vehicle_counts"].keys()):
            cv2.putText(annotated, f"{name}: {counter_state['vehicle_counts'][name]}", (10, overlay_y), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
            overlay_y += 26

        return annotated

    except Exception as e:
        print(f"[ERROR] Error processing frame: {e}")
        return frame


# =============================================
# WORKER THREAD
# =============================================

def counter_worker():
    """Background worker that continuously processes frames"""
    fps_target = 30
    frame_duration = 1.0 / fps_target
    last_time = time.time()

    while True:
        time.sleep(0.01)  # Small sleep to avoid busy loop

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
    """Background worker that samples current total count for last-10-minute stats"""
    while True:
        try:
            with counter_state["lock"]:
                total = sum(counter_state["vehicle_counts"].values())
                _record_counter_history(total)
        except Exception as e:
            print(f"[STATS] Error recording counter history: {e}")
        time.sleep(1)


# =============================================
# FASTAPI APP
# =============================================

app = FastAPI(
    title="Vehicle Counter API",
    description="Real-time vehicle counting system with YOLOv8 and ByteTrack",
    version="1.0.0"
)

# Add CORS middleware
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.on_event("startup")
async def startup_event():
    """Initialize model and start worker thread"""
    if YOLO is None:
        raise RuntimeError("ultralytics YOLO not installed. Install with: python -m pip install ultralytics")

    print("[INFO] Loading YOLO model...")
    model_path = "yolov8x.engine" if os.path.exists("yolov8x.engine") else "yolov8x.pt"
    counter_state["model"] = YOLO(model_path)
    print(f"[INFO] YOLO model loaded from {model_path}.")

    line_points, tracked_class_ids, labels = _reload_config_values()
    if not line_points:
        line_points = []
    counter_state["line_points"] = [tuple(map(int, p)) for p in line_points[:2]]
    counter_state["tracked_class_ids"] = tracked_class_ids
    counter_state["labels"] = labels

    # Start worker thread
    worker_thread = threading.Thread(target=counter_worker, daemon=True)
    worker_thread.start()

    stats_thread = threading.Thread(target=stats_worker, daemon=True)
    stats_thread.start()


@app.post("/start")
async def start_counter():
    """Start the vehicle counter"""
    if counter_state["stream"] is None:
        try:
            video_url = get_youtube_stream_url(app_config.YOUTUBE_URL)
            counter_state["stream"] = SmoothStream(video_url).start()
            print("[INFO] Stream started")
        except Exception as e:
            raise HTTPException(status_code=500, detail=f"Failed to start stream: {str(e)}")

    counter_state["running"] = True
    return {"status": "running", "message": "Counter started"}


@app.post("/stop")
async def stop_counter():
    """Stop the vehicle counter"""
    counter_state["running"] = False
    return {"status": "stopped", "message": "Counter stopped"}


@app.post("/reset")
async def reset_counter():
    """Reset all counters to 0"""
    with counter_state["lock"]:
        counter_state["vehicle_counts"] = defaultdict(int)
        counter_state["counted_track_ids"] = set()
        counter_state["previous_centers"] = {}
    print("[INFO] Counter reset")
    return {"status": "reset", "message": "All counters reset to 0", "counts": {}}


@app.get("/get_counter")
async def get_counter():
    """Get current vehicle counts"""
    return _snapshot_counter_stats()


@app.get("/counter_stats")
async def counter_stats():
    """Return rolling 10-minute counter statistics for betting previews"""
    return _snapshot_counter_stats()


@app.get("/stream_preview")
async def stream_preview():
    """Return current frame as PNG image"""
    if counter_state["current_frame"] is None:
        raise HTTPException(status_code=503, detail="Stream not initialized or no frame available")

    with counter_state["lock"]:
        frame = counter_state["current_frame"].copy()

    # Encode frame as PNG
    success, buffer = cv2.imencode('.png', frame)
    if not success:
        raise HTTPException(status_code=500, detail="Failed to encode frame")

    return Response(content=buffer.tobytes(), media_type="image/png")


def generate_stream():
    """Generator for streaming MJPEG video"""
    while True:
        if counter_state["current_frame"] is None:
            time.sleep(0.05)
            continue

        with counter_state["lock"]:
            frame = counter_state["current_frame"].copy()

        # Encode frame as JPEG
        success, buffer = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, 80])
        if success:
            frame_bytes = buffer.tobytes()
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n'
                   b'Content-Length: ' + str(len(frame_bytes)).encode() + b'\r\n\r\n'
                   + frame_bytes + b'\r\n')
        time.sleep(0.033)  # ~30 fps


@app.get("/stream")
async def stream():
    """Stream live labeled video as MJPEG"""
    if counter_state["model"] is None:
        raise HTTPException(status_code=503, detail="Model not initialized")

    return StreamingResponse(
        generate_stream(),
        media_type="multipart/x-mixed-replace; boundary=frame"
    )


@app.get("/health")
async def health():
    """Health check endpoint"""
    return {
        "status": "healthy",
        "model_loaded": counter_state["model"] is not None,
        "running": counter_state["running"],
        "stream_active": counter_state["stream"] is not None
    }


@app.get("/")
async def root():
    """API root with documentation links"""
    return {
        "message": "Vehicle Counter API",
        "docs": "/docs",
        "endpoints": {
            "start": "POST /start",
            "stop": "POST /stop",
            "reset": "POST /reset",
            "get_counter": "GET /get_counter",
            "stream_preview": "GET /stream_preview",
            "stream": "GET /stream",
            "health": "GET /health"
        }
    }


if __name__ == "__main__":
    print("[INFO] Starting FastAPI server on http://localhost:8000")
    print("[INFO] Documentation available at http://localhost:8000/docs")
    uvicorn.run(app, host="0.0.0.0", port=8000)

