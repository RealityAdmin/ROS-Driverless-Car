import spidev
import cv2
import numpy as np
import time

while True:
    # Forward
    spi.xfer([0x66])
    print(chr(0x66))
    time.sleep(1)
