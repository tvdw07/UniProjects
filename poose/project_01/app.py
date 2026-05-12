import cv2
import yt_dlp
import os

# Detection & tracking imports: keep only YOLO + ByteTrack
try:
    from ultralytics import YOLO
except Exception:
    YOLO = None

import torch
print(torch.cuda.is_available())
print(torch.version.cuda)
print(torch.cuda.device_count())

# (Removed TensorFlow remnants: YOLO + ByteTrack used instead)

# -----------------------------
# COCO LABELS (relevant classes)
# -----------------------------
LABELS = {
    3: "car",
    6: "bus",
    8: "truck",
    4: "motorcycle"
}

def get_youtube_stream_url(youtube_url):
    ydl_opts = {'format': 'best', 'quiet': True}
    with yt_dlp.YoutubeDL(ydl_opts) as ydl:
        info = ydl.extract_info(youtube_url, download=False)
        return info['url']

# (CentroidTracker removed — using ByteTrack only)


# -----------------------------
# LOAD DETECTION MODEL (YOLO)
# -----------------------------
if YOLO is None:
    raise RuntimeError("ultralytics YOLO not installed. Install with: python -m pip install ultralytics")

print("[INFO] Loading YOLO model...")
model_path = "yolov8m.engine" if os.path.exists("yolov8m.engine") else "yolov8m.pt"
model = YOLO(model_path)
print(f"[INFO] YOLO model loaded from {model_path}.")

# -----------------------------
# VIDEO STREAM
# -----------------------------

class ThreadedCamera:
    def __init__(self, src):
        self.cap = cv2.VideoCapture(src)
        self.cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
        if not self.cap.isOpened():
            raise RuntimeError("[ERROR] Could not open video source.")
        self.ret, self.frame = self.cap.read()
        self.stopped = False
        import threading
        self.thread = threading.Thread(target=self.update, args=())
        self.thread.daemon = True
        self.thread.start()

    def update(self):
        while not self.stopped:
            if not self.cap.isOpened():
                break
            ret, frame = self.cap.read()
            if ret:
                self.frame = frame
            else:
                self.stopped = True

    def read(self):
        return self.ret, self.frame

    def release(self):
        self.stopped = True
        self.thread.join()
        self.cap.release()

YOUTUBE_URL = "https://www.youtube.com/watch?v=M3EYAY2MftI"
VIDEO_URL = get_youtube_stream_url(YOUTUBE_URL)

cap = ThreadedCamera(VIDEO_URL)

# Using ultralytics' built-in tracking (ByteTrack) via model.track
print("[INFO] Using ultralytics.track with ByteTrack (tracker config expected, e.g. 'bytetrack.yaml')")

# -----------------------------
# MAIN LOOP
# -----------------------------
while True:
    ret, frame = cap.read()
    if not ret:
        print("[ERROR] Frame not received")
        break

    # -----------------------------
    # DETECTION + TRACKING via ultralytics model.track (uses ByteTrack YAML config)
    # -----------------------------
    results = model.track(
        frame,
        persist=True,
        tracker="bytetrack.yaml",
        conf=0.35,
        classes=[2,3,5,7],
        device=0,
        half=True,
        imgsz=640,
        verbose=False,
    )
    res = results[0]

    # Use native plot to avoid CPU transfer
    annotated_frame = res.plot()

    cv2.imshow("Vehicle Detection + Tracking", annotated_frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()