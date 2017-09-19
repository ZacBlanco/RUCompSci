#!/usr/bin/env python
import numpy as np
import cv2
from matplotlib import pyplot as plt



# import modules used here -- sys is a very standard one
import sys

refPt = []
image = np.zeros((512, 512, 3), np.uint8)
windowName = 'HW Window';
lx = -1
ly = -1
def click_and_keep(event, x, y, flags, param):
	# grab references to the global variables
	global refPt, image,lx,ly
 
	# if the left mouse button was clicked, record the starting
	# (x, y) coordinates 
	# performed
	if event == cv2.EVENT_LBUTTONDOWN:
		refPt.append((x, y)) # I modified this line to add all the refPt to a list instead of setting it equal
		print(x,y)
		lx = x
		ly = y
def warp():
	global image
	# Select End Points of foreshortened window or billboard
	# print(refPt)
	if len(refPt) < 1:
		print("Not enough points selected. Can't estimate homography.")
		return

	# Set the corresponding point in the frontal view as reference points
	# We use the first selected point to be the reference (top left)
	homPts = [refPt[0], (refPt[0][0]+250, refPt[0][1]), (refPt[0][0], refPt[0][1]+250), (refPt[0][0]+250, refPt[0][1]+250)]
	# homPts = [(0, 0), (image.shape[1], 0), (0, image.shape[0]), (image.shape[1], image.shape[0]) ]
	homPts = homPts[:len(refPt)]
	print(refPt)
	print(homPts)
	for pt in homPts:
		image = cv2.circle(image,(pt[0], pt[1]), 12, (0,0,255), -1);
		cv2.imshow(windowName, image)
		cv2.waitKey(1)
	# Estimate the homography
	Amat = []
	for i in range(len(refPt)):
		rp = refPt[i]
		hp = homPts[i]
		Amat.append([-hp[0], -hp[1], -1, 0, 0, 0, rp[0]*hp[0], rp[0]*hp[1], rp[0]])
		Amat.append([0, 0, 0, -hp[0], -hp[1], -1, rp[1]*hp[0], hp[1]*rp[1], rp[1]])
	Amat = np.matrix(Amat)
	np.set_printoptions(linewidth=1000)
	u, s, v = np.linalg.svd(Amat)
	v = v.H # The V returned is really V.T
	hom = v[:,-1]
	hom.shape = (3, 3)
	# Warp the image
	def disp_mat(data):
		cv2.imshow('img', data)
		cv2.waitKey(10000)
		cv2.destroyAllWindows()
	
	mapx = np.zeros(image.shape[:2], np.float32)
	mapy = np.zeros(image.shape[:2], np.float32)

	for pt in refPt:
		v = np.array([pt[0], pt[1], 1])
		print(v)
		newpt = hom.dot(v.T)
		print(newpt/newpt.T[2])
	print(image.shape)
	for y in range(image.shape[0]): # rows
		for x in range(image.shape[1]): # cols
			vec = np.array([x, y, 1]).T
			pts = hom.dot(vec).T
			pts = pts/pts[2]
			mapx[y, x] = pts[0]
			mapy[y, x] = pts[1]
	I2 = np.zeros(image.shape, np.float32)
	I2 = cv2.remap(image, mapx, mapy, 1)
	cv2.imwrite('test.jpg', I2)


	#Crop the image

# Gather our code in a main() function
def main():
	# Read Image
	print("IMPORTANT INFORMATION")
	print("When selecting points please select them in the following order:")
	print("Top Left")
	print("Top Right")
	print("Bottom Left")
	print("Bottom Right")
	input("Press any key to continue...")
	image = cv2.imread('ts.jpg',1);
	i = image
	# image  = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
	cv2.namedWindow(windowName)
	cv2.setMouseCallback(windowName, click_and_keep)
 
# keep looping until the 'q' key is pressed

	while True:
	# display the image and wait for a keypress
		image = cv2.circle(image,(lx,ly), 10, (0,255,255), -1);
		cv2.imshow(windowName, image)
		key = cv2.waitKey(1) & 0xFF
 
	# if the 'c' key is pressed, break from the loop
		if key == ord("c"):
			break

	global image
	image = i
	warp()
	# Close the window will exit the program
	cv2.destroyAllWindows()

# Standard boilerplate to call the main() function to begin
# the program.
if __name__ == '__main__':
    main()




