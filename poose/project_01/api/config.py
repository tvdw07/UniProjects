# -----------------------------
# COCO LABELS (relevant classes)
# -----------------------------
LABELS = {
    3: "car",
    6: "bus",
    8: "truck",
    4: "motorcycle",
}

# Ultralytics COCO class ids used by the tracker
TRACKED_CLASS_IDS = [2, 3, 5, 7]

# Persistent counting line position in pixel coordinates.
# If COUNTING_LINE_POINTS is set, the app uses the 2-point line.
COUNTING_LINE_POINTS = [(1533, 363), (1021, 1077)]

YOUTUBE_URL = "https://www.youtube.com/live/6dp-bvQ7RWo?si=inGVO0ahx3pV5ISU"

LOCATIONS = {'bangkok': {'line': [(1390, 369), (741, 1072)], 'url': 'https://www.youtube.com/live/UemFRPrl1hk'},
 'california': {'line': [(1839, 1036), (102, 574)], 'url': 'https://www.youtube.com/watch?v=sTF-6_xinUU'},
 'koh_samui': {'line': [(642, 1053), (21, 717)], 'url': 'https://www.youtube.com/live/VR-x3HdhKLQ'},
 'london': {'line': [(1596, 751), (591, 484)], 'url': 'https://www.youtube.com/watch?v=M3EYAY2MftI'},
 'tokyo': {'line': [(1539, 346), (754, 1077)], 'url': 'https://www.youtube.com/live/6dp-bvQ7RWo'}}
CURRENT_LOCATION = 'bangkok'
