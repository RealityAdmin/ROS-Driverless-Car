import spidev
import cv2
import numpy as np

# Setup SPI
spi = spidev.SpiDev()
spi.open(0, 0)
spi.max_speed_hz = 115200

# Setup Cascade
cascade_src = 'Stopsign_HAAR_19Stages.xml'
stopsigns_cascade = cv2.cascadeClassifier(cascade_src)

cooldown = 0

def DetectStopSign(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    stopsigns = stopsigns_cascade.detectMultiScale(gray, 1.1, 1)

    for (x,y,w,h) in stopsigns:
        cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),2) 
        
    cv2.imshow("Stop Signs", frame)
    
    if len(stopsigns) > 0:
        print("Stop Sign Detected")
        return True
    else:
        return False

cam = cv2.VideoCapture(0)
while True:
    # Get the status of capture and the current frame
    ret, frame = cam.read()
    if ret:
        # The frame is the image we manipulate
        if cooldown == 0 and DetectStopSign(frame):
            time.sleep(3)
            cooldown = 10
        if cooldown > 0:
            cooldown -= 1
            spi.xfer([0x66])
            print(chr(0x66))
        edges = cv2.Canny(frame, 150,175)
        lines = cv2.HoughLinesP(edges, 1, np.pi/180, 10, None, 0, 150)
        avg_slope = 0
        if lines is not None:
            for line in lines:
                for x1, y1, x2, y2 in line:
                    # Minimum line length
                    if (x2-x1) ** 2 + (y2-y1) ** 2 < 10 ** 2:
                        continue
                    cv2.line(frame, (x1, y1), (x2, y2), (0, 0, 255), 5)
                    if x2 - x1 == 0:
                        x2 += 0.01
                    avg_slope += (y2-y1)/(x2-x1)
            avg_slope /= len(lines)
            # Steering is calculated based on the average slope of the lines
            if abs(avg_slope) > 1:
                # Forward
                spi.xfer([0x66])
                print(chr(0x66))
            elif avg_slope > 0:
                # Left
                spi.xfer([0x6C])
                print(chr(0x6C))
            else:
                # Right
                spi.xfer([0x72])
                print(chr(0x72))
        cv2.imshow("Robot", frame)

        # Press Q to break the program
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

# Stop the footage and close all windows
cam.release()
cv2.destroyAllWindows
