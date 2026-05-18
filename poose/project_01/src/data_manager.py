import json
import threading
from config import DATA_JSON_PATH

_file_lock = threading.Lock()

def load_data():
    """Lädt die dynamischen Daten aus der JSON-Datei."""
    with _file_lock:
        if not DATA_JSON_PATH.exists():
            return {"current_location": "tokyo", "locations": {}}
        with open(DATA_JSON_PATH, "r", encoding="utf-8") as f:
            return json.load(f)

def save_data(data):
    """Speichert die dynamischen Daten thread-sicher in die JSON-Datei."""
    with _file_lock:
        with open(DATA_JSON_PATH, "w", encoding="utf-8") as f:
            json.dump(data, f, indent=2, ensure_ascii=False)

def get_current_location_data():
    """Gibt Name, Linie und URL der aktiven Location zurück."""
    data = load_data()
    loc = data.get("current_location", "tokyo")
    loc_data = data.get("locations", {}).get(loc, {"line": [], "url": ""})
    return loc, loc_data.get("line", []), loc_data.get("url", "")

def update_line_points(loc, points):
    """Aktualisiert die Zähllinie einer Location."""
    data = load_data()
    if loc in data.get("locations", {}):
        data["locations"][loc]["line"] = [list(p) for p in points]
        save_data(data)

def update_current_location(loc):
    """Wechselt die aktive Location im Datensatz."""
    data = load_data()
    if loc in data.get("locations", {}):
        data["current_location"] = loc
        save_data(data)