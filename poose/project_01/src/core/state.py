import threading
import time
from collections import defaultdict

class CounterState:
    def __init__(self):
        self.lock = threading.Lock()
        self.running = False
        self.show_boxes = True
        self.vehicle_counts = defaultdict(int)
        self.counted_track_ids = set()
        self.previous_centers = {}
        self.line_points = []
        self.current_location = "tokyo"
        self.history = []
        self.current_frame = None

    def reset(self):
        with self.lock:
            self.vehicle_counts.clear()
            self.counted_track_ids.clear()
            self.previous_centers.clear()

    def record_history(self, max_age_seconds=600):
        now_ts = time.time()
        with self.lock:
            total = sum(self.vehicle_counts.values())
            self.history.append({"ts": now_ts, "total": total})
            cutoff = now_ts - max_age_seconds
            self.history = [entry for entry in self.history if entry["ts"] >= cutoff]

    def get_snapshot(self):
        with self.lock:
            counts = dict(self.vehicle_counts)
            running = self.running
            line_points = list(self.line_points)
            history = list(self.history)
            loc = self.current_location

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
            "current_location": loc
        }
