import cv2
import yt_dlp

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
model = YOLO("yolov8m.pt")  # small model, will be downloaded if missing
print("[INFO] YOLO model loaded.")

# -----------------------------
# VIDEO STREAM
# -----------------------------

YOUTUBE_URL = "https://www.youtube.com/watch?v=M3EYAY2MftI"
VIDEO_URL = get_youtube_stream_url(YOUTUBE_URL)


cap = cv2.VideoCapture(VIDEO_URL)
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
if not cap.isOpened():
    raise RuntimeError(
        "[ERROR] Could not open video source. Use a direct stream URL (e.g., RTSP/MJPEG/MP4), not a webpage URL."
    )

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


    h, w = frame.shape[:2]
    if w > 1280:
        frame = cv2.resize(frame, (1280, 720))

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
        verbose=False,
    )
    res = results[0]

    tracked_objects = []  # list of (id, x1,y1,x2,y2)
    if hasattr(res, 'boxes') and len(res.boxes) > 0:
        boxes = res.boxes
        xyxy = boxes.xyxy.int().cpu().tolist()
        ids = None
        if boxes.id is not None:
            try:
                ids = boxes.id.int().cpu().tolist()
            except Exception:
                ids = None
        for i, b in enumerate(xyxy):
            x1, y1, x2, y2 = b
            tid = int(ids[i]) if ids is not None else i
            tracked_objects.append((tid, x1, y1, x2, y2))

    # draw tracked objects with boxes + ids
    for tid, x1, y1, x2, y2 in tracked_objects:
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0,255,0), 2)
        cv2.putText(frame, f"ID {tid}", (x1, y1 - 8), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0,255,0), 2)

    cv2.imshow("Vehicle Detection + Tracking", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()