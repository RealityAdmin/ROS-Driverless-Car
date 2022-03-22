import cv2
import numpy as np
import matplotlib.pyplot as plt

img = cv2.imread('road.jpg',0)
edges = cv2.Canny(img,400,450)
lines = cv2.HoughLinesP(edges, 1, np.pi/180, 30, None, 0, 100)
if lines is not None:
    for i in range(0, len(lines)):
        l = lines[i][0]
        cv2.line(img, (l[0],l[1]), (l[2],l[3]), (0,0,255), 10)
cv2.imshow("Img", img)

cv2.waitKey(0)
cv2.destroyAllWindows()

