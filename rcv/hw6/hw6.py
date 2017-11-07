import numpy as np
import cv2
import time
from math import cos, sin, radians

sx = sy = 1
f = 100 # focal length
ox = oy = 200 # shift
Mint = np.matrix([[f/sx, 0, 256],
                    [0, f/sy, 256],
                    [0,    0,  1]])

def disp(img):
    cv2.imshow("img", img)
    cv2.waitKey(1)

def disp_key(data, key, width=None, height=None):
	cv2.imshow('img', data)
	if width != None and height != None:
		cv2.resizeWindow('img', int(width), int(height))
	while True:
		k = cv2.waitKey(50) & 0xFF
		if k == ord(key):
			break