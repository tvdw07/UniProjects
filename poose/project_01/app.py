import cv2
import yt_dlp
import os
import time
import threading
from collections import deque

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
model_path = "yolov8x.engine" if os.path.exists("yolov8x.engine") else "yolov8x.pt"
model = YOLO(model_path)
print(f"[INFO] YOLO model loaded from {model_path}.")

# -----------------------------
# VIDEO STREAM
# -----------------------------

class SmoothStream:
    def __init__(self, url):
        self.cap = cv2.VideoCapture(url)
        # WICHTIG: Buffer im Backend klein halten
        self.cap.set(cv2.CAP_PROP_BUFFERSIZE, 2)
        self.q = deque(maxlen=300) # 10s Puffer bei 30fps
        self.stopped = False

    def start(self):
        t = threading.Thread(target=self.update, args=())
        t.daemon = True
        t.start()
        return self

    def update(self):
        while not self.stopped:
            ret, frame = self.cap.read()
            if not ret:
                break
            self.q.append(frame)

    def get_frame(self):
        return self.q[0] if len(self.q) > 0 else None

    def pop_frame(self):
        if len(self.q) > 0:
            return self.q.popleft()
        return None

YOUTUBE_URL = "https://www.youtube.com/live/6dp-bvQ7RWo?si=inGVO0ahx3pV5ISU"
VIDEO_URL = get_youtube_stream_url(YOUTUBE_URL)

# Instanz starten
stream = SmoothStream(VIDEO_URL).start()

# Using ultralytics' built-in tracking (ByteTrack) via model.track
print("[INFO] Using ultralytics.track with ByteTrack (tracker config expected, e.g. 'bytetrack.yaml')")

# FPS-Kontrolle
fps_target = 30
frame_duration = 1.0 / fps_target
last_time = time.time()

# -----------------------------
# MAIN LOOP
# -----------------------------
while True:
    current_time = time.time()

    # Warte bis der nächste Frame laut Zeitplan dran ist
    if current_time - last_time < frame_duration:
        continue

    frame = stream.get_frame() # Schau in die Vergangenheit (Puffer-Anfang)

    if frame is not None:
        # Falls der Puffer ZU voll wird (> 90%), überspringe Frames um Lags aufzuholen
        if len(stream.q) > 280:
            for _ in range(5): stream.pop_frame()

        results = model.track(
            frame,
            persist=True,
            tracker="bytetrack.yaml",
            conf=0.3,
            classes=[2,3,5,7],
            device=0,
            half=True,
            verbose=False
        )

        # Anzeige
        annotated = results[0].plot()
        cv2.imshow("RTX 4070 - No Lag Mode", annotated)

        # Den verarbeiteten Frame nun aus dem Puffer entfernen
        stream.pop_frame()
        last_time = current_time

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

stream.stopped = True
cv2.destroyAllWindows()