from pathlib import Path

# Pfade
BASE_DIR = Path(__file__).resolve().parent
DATA_JSON_PATH = BASE_DIR / "data.json"

# Fenster- & Anzeigeoptionen
WINDOW_NAME = "RTX 4070 - No Lag Mode"
DISPLAY_WIDTH = 1280
DISPLAY_HEIGHT = 720
FPS_TARGET = 30

# Modell-Optionen
MODEL_PATH = "yolov8x.engine" if (BASE_DIR / "yolov8x.engine").exists() else "yolov8x.pt"
TRACKER_CONFIG = "bytetrack.yaml"
DEFAULT_CONFIDENCE = 0.28

# Offizielle COCO Labels und IDs
LABELS = {
    2: "car",
    3: "motorcycle",
    5: "bus",
    7: "truck",
}

TRACKED_CLASS_IDS = list(LABELS.keys())