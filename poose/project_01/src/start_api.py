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
from core.state import CounterState
from core.pipeline import VehiclePipeline

from contextlib import asynccontextmanager
import sys
import asyncio

try:
    from ultralytics import YOLO
except Exception:
    YOLO = None

state_instance = CounterState()
pipeline_instance = None

def get_pipeline():
    global pipeline_instance
    if pipeline_instance is None:
        pipeline_instance = VehiclePipeline(state_instance)
    return pipeline_instance

if sys.platform == "win32":
    try:
        from asyncio.proactor_events import _ProactorBasePipeTransport

        # Keep a reference to the original method
        _original_call_connection_lost = _ProactorBasePipeTransport._call_connection_lost

        def _silence_connection_lost(self, exc):
            try:
                _original_call_connection_lost(self, exc)
            except ConnectionResetError:
                pass

        _ProactorBasePipeTransport._call_connection_lost = _silence_connection_lost
    except ImportError:
        pass

@asynccontextmanager
async def lifespan(app: FastAPI):
    if YOLO is None:
        raise RuntimeError("ultralytics YOLO not installed.")

    print(f"[INFO] Initializing Pipeline...")
    pipeline = get_pipeline()

    loc, line_points, _ = data_manager.get_current_location_data()
    with state_instance.lock:
        state_instance.line_points = [tuple(map(int, p)) for p in line_points[:2]]
        state_instance.current_location = loc

    pipeline.start_workers()
    yield

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
    redoc_url=None,
    lifespan=lifespan
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.post("/start")
async def start_counter():
    """Starts the vehicle counter stream. If the stream is not initialized, it sets it up and begins counting."""
    pipeline = get_pipeline()
    if pipeline.stream is None:
        try:
            pipeline.start_stream()
            print("[INFO] Stream started")
        except Exception as e:
            raise HTTPException(status_code=500, detail=f"Failed to start stream: {str(e)}")
    else:
        with state_instance.lock:
            state_instance.running = True

    return {"status": "running", "message": "Counter started"}


@app.post("/location/{loc_id}")
async def set_location(loc_id: str):
    """Sets a new location for the vehicle counter, updating the camera view and counting line based on location data."""
    pipeline = get_pipeline()
    success = pipeline.load_location(loc_id)
    if not success:
        raise HTTPException(status_code=404, detail="Location not found")

    return {"status": "success", "location": loc_id}


@app.post("/stop")
async def stop_counter():
    """Stops the vehicle counter stream from processing frames."""
    get_pipeline().stop_stream()
    return {"status": "stopped"}


@app.post("/reset")
async def reset_counter():
    """Resets the statistics and counts of tracked vehicles back to zero."""
    state_instance.reset()
    return {"status": "reset"}


@app.get("/get_counter")
async def get_counter():
    """Retrieves the current state and totals of the tracked vehicles."""
    return state_instance.get_snapshot()


@app.get("/counter_stats")
async def counter_stats():
    """Retrieves detailed statistics and history of the vehicle counting process."""
    return state_instance.get_snapshot()


@app.get("/stream_preview")
async def stream_preview():
    """Returns a single image captured from the current frame of the stream."""
    with state_instance.lock:
        frame = state_instance.current_frame
    if frame is None:
        raise HTTPException(status_code=503, detail="No frame available")

    # Always copy safely
    frame = frame.copy()

    success, buffer = cv2.imencode('.png', frame)
    if not success:
        raise HTTPException(status_code=500, detail="Encoding failed")
    return Response(content=buffer.tobytes(), media_type="image/png")


def generate_stream():
    while True:
        with state_instance.lock:
            frame = state_instance.current_frame

        if frame is None:
            time.sleep(0.05)
            continue

        frame = frame.copy()

        success, buffer = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, 80])
        if success:
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + buffer.tobytes() + b'\r\n')
        time.sleep(0.033)


@app.get("/stream")
async def stream():
    """Provides a continuous MJPEG stream of the annotated video feed."""
    pipeline = get_pipeline()
    if pipeline.model is None:
        raise HTTPException(status_code=503, detail="Model not initialized")
    return StreamingResponse(generate_stream(), media_type="multipart/x-mixed-replace; boundary=frame")


@app.get("/health")
async def health():
    """Health check endpoint to verify the API is running and returns basic status."""
    return {"status": "healthy", "running": state_instance.running}


@app.get("/")
async def root():
    """Root endpoint for basic API information."""
    return {"message": "Vehicle Counter API", "docs": "/docs"}


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)