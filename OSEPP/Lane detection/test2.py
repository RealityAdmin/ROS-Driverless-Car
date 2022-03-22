import cv2
import numpy as np
import matplotlib.pyplot as plt

#Declare our camera as the footage from the video
cam = cv2.VideoCapture('roadVideo.mp4')

while True:
    #Get the status of capture and the current frame
    ret, frame = cam.read()
    if ret:
        #The frame is the image we manipulate
        road = frame[140:203, 0:424]
        edges = cv2.Canny(road,400,450)
        lines = cv2.HoughLinesP(edges, 1, np.pi/180, 10, None, 0, 150)
        if lines is not None:
            for i in range(0, len(lines)):
                l = lines[i][0]
                cv2.line(frame, (l[0],l[1]+140), (l[2],l[3]+140), (0,0,255), 5)
        cv2.imshow("Img", frame )

        #Press Q to break the program
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

#Stop the footage and close all windows
cam.release()
cv2.destroyAllWindows()

