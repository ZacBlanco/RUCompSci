import numpy as np
import cv2
import time
from math import cos, sin, radians
from matplotlib import pyplot as plt

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

def problem1():
	imgs = ["img_left.jpg", "img_right.jpg"]
	sift = cv2.xfeatures2d.SIFT_create()
	ims = [cv2.imread(x) for x in imgs]
	grays = [cv2.cvtColor(x, cv2.COLOR_BGR2GRAY) for x in ims]
	kp_des = [sift.detectAndCompute(x, None) for x in grays]
	bf = cv2.BFMatcher()
	matches = bf.knnMatch(kp_des[0][1], kp_des[1][1], k=2)
	good = []
	for m, n in matches:
		if m.distance < 0.75 * n.distance:
			good.append([m])
	im3 = cv2.drawMatchesKnn(grays[0], kp_des[0][0], grays[1], kp_des[1][0], good, flags=2) 
	plt.imshow(im3)
	plt.show()	
def main():
	problem1()


if __name__ == "__main__":
	main()