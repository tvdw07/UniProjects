import cv2
import yt_dlp
import threading
from collections import deque

def get_youtube_stream_url(youtube_url):
    ydl_opts = {'format': 'best', 'quiet': True}
    with yt_dlp.YoutubeDL(ydl_opts) as ydl:
        info = ydl.extract_info(youtube_url, download=False)
        return info['url']

class SmoothStream:
    def __init__(self, url):
        self.cap = cv2.VideoCapture(url)
        self.cap.set(cv2.CAP_PROP_BUFFERSIZE, 2)
        self.q = deque(maxlen=300)
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