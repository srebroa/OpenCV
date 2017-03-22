import cv2
import os
import threading
import numpy as np
import urllib

# Get user supplied values
cascPath = "haarcascade_frontalface_default.xml" #sys.argv[1]
# Create the haar cascade
faceCascade = cv2.CascadeClassifier(cascPath)

# initialize the camera Dlink DCS5020L
stream=urllib.urlopen('http://login:password@192.168.0.11/video.cgi?.mjpg')


def face_detection_thread(frame, gray):
    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.1,
        minNeighbors=5,
        minSize=(80, 80),
    )
    # Draw a rectangle around the faces
    for (x, y, w, h) in faces:
        Cx = (int)(x + w / 2)
        Cy = (int)(y + h / 2)
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        centerCoordinatesStr = str(Cx) + ', ' + str(Cy)
        cv2.putText(frame, centerCoordinatesStr, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 1,
                    (0, 255, 0), 2)
    # Display the resulting frame
    cv2.imshow('MJPG Video Face Detection', frame)

def main():
    bytes = ''
    while True:
        # read mjpeg frame
        bytes += stream.read(1024)
        a = bytes.find('\xff\xd8')
        b = bytes.find('\xff\xd9')
        if a != -1 and b != -1:
            jpg = bytes[a:b + 2]
            bytes = bytes[b + 2:]
            frame = cv2.imdecode(np.fromstring(jpg, dtype=np.uint8), cv2.IMREAD_COLOR)
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            t1 = threading.Thread(name='Face detection', target=face_detection_thread(frame,gray))
            t1.start()
            t1.join()
            if cv2.waitKey(1) == 27: #ESC
                break

    # When everything is done just release the capture
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()