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
		# print(x,y)
		lx = x
		ly = y
def disp_mat(data, key, width=None, height=None):
	cv2.imshow('img', data)
	if width != None and height != None:
		cv2.resizeWindow('img', int(width), int(height))
	while True:
		k = cv2.waitKey(50) & 0xFF
		if k == ord(key):
			break
	cv2.destroyWindow("img")

def calcmapPt(x, y, hom):
	vec = np.array([x, y, 1]).T # The original point vector
	pts = np.linalg.inv(hom).dot(vec).T # Calculate the new points

	pts = pts/pts[2] # divide by ~w' to get x' y'
	return (pts[0], pts[1])

def warp(image, orig, mapped, pushed_img=None):
	'''Warps the image by estimating the homography from orig -> mapped points.'''

	# Create the matrix used to estimate homography
	Amat = []
	for i in range(len(orig)):
		rp = orig[i]
		hp = mapped[i]
		Amat.append([-rp[0], -rp[1], -1, 0, 0, 0, hp[0]*rp[0], hp[0]*rp[1], hp[0]])
		Amat.append([0, 0, 0, -rp[0], -rp[1], -1, hp[1]*rp[0], rp[1]*hp[1], hp[1]])
	Amat = np.matrix(Amat) # turn into numpy object.


	# Get the homography from the null space of A (last col of V)
	u, s, v = np.linalg.svd(Amat)
	v = v.H # The V returned is really V.T
	hom = v[:,-1]
	hom.shape = (3, 3) # Reshape back into the 3x3

	# numpy arrays for mappings
	mapx = np.zeros(image.shape[:2], np.float32)
	mapy = np.zeros(image.shape[:2], np.float32)

	# Calculate the mapping using the homography matrix
	for y in range(image.shape[0]): # rows
		for x in range(image.shape[1]): # cols
			vec = np.array([x, y, 1]).T # The original point vector
			pts = np.linalg.inv(hom).dot(vec).T # Calculate the new points
			pts = pts/pts[2] # divide by ~w' to get x' y'
			mapx[y, x] = pts[0] # Set corresponding map points
			mapy[y, x] = pts[1]
	# performs the remapping and display the new image
	newimg = np.zeros(image.shape, np.float32)
	newimg = cv2.remap(image, mapx, mapy, 1) # linear interpolation
	return newimg

def get_points(selected_points):
	'''Splits a list in half. The first half are the original image points.
		The second half is the location each should be mapped to.

		i.e. [1, 2, 3, 4, 5, 6] gives 
		orig =   [1, 2, 3]
		mapped = [4, 5, 6]
	Args:
		selected_points (list): A list of selected points

	Returns:
		(tuple): (originalPts, mappedPts)
	'''
	if len(selected_points) % 2 == 0:
		mapped = selected_points[int(len(selected_points)/2):]
		orig = selected_points[:int(len(selected_points)/2)]
	else:
		raise ArgumentError("Can't split list of points. Even number must be selected")

	return (orig, mapped)

def problem2():
	######################## PROBLEM 2 #############################
	# Select End Points of foreshortened window or billboard
	global refPt, image
	# Add the mapping points
	refPt.append((100, 100))
	refPt.append((1200, 100))
	refPt.append((1200, 800))
	refPt.append((100, 800))
	orig, mapped = get_points(refPt)
	# Set the corresponding point in the frontal view as reference points
	
	# Draw the points where we're going to map to
	for pt in mapped:
		image = cv2.circle(image,(pt[0], pt[1]), 12, (0,0,255), -1);
		cv2.imshow(windowName, image)
		cv2.waitKey(1)
	
	disp_mat(warp(image, orig, mapped), "c")
	############# PROBLEM 2 ####################

def problem3():
	############# PROBLEM 3 ####################
	# Read in the new image but we're going to expand it to the size of original
	testimg = cv2.imread("testimg.jpg")
	blank = np.zeros(image.shape, np.uint8)
	rows = min(testimg.shape[0], image.shape[0])
	cols = min(testimg.shape[1], image.shape[1])
	testimg = testimg[:rows,:cols]
	blank[0:rows,0:cols] = testimg
	# disp_mat(testimg, "c")
	
	# Image was now expanded
	rows, cols = testimg.shape[:2] # size of the original image
	points = [(0, 0), (cols, 0), (cols, rows), (0, rows)]
	[points.append(refPt[i]) for i in range(4)]
	orig, mapped = get_points(points)
	newimg = warp(blank, orig, mapped)

	# From OpenCV Tutorials - creating an Alpha mask to combine images
	newimg_gray = cv2.cvtColor(newimg, cv2.COLOR_BGR2GRAY) # converts the image to grayscale
	ret, mask = cv2.threshold(newimg_gray, 1, 255, cv2.THRESH_BINARY) # threshold the image
	mask_inv = cv2.bitwise_not(mask)

	# mask the actual image (and the inverse)
	img_bg = cv2.bitwise_and(image, image, mask=mask_inv)
	img_fg = cv2.bitwise_and(newimg, newimg, mask=mask)

	# Add images and display
	newimg = cv2.add(img_bg, img_fg)
	disp_mat(newimg, "c")
	############# PROBLEM 3 ####################

def problem4():
	l = 512 # The image size
	size = (l,l) # Size of the new image
	sx = sy = 1 # problem parameter
	f = 100 # focal length
	ox = oy = 200 # shift

	# Problem parameter (Internal camera matrix)
	Mint = np.matrix([[-.707, -.707, 0,   3],
					  [.707, -.707,  0, 0.5],
					  [0,     0,     1,   3]])
	# Problem Parameter (External Camera Matrix)
	Mext = np.matrix([[f/sx, 0, ox],
					  [0, f/sy, oy],
					  [0,    0,  1]])

	M = Mext.dot(Mint) # results in a 3x4 Matrix (camera matrix)

	# House size parameter
	sz = 200
	house_points = [
		[(0, 0, 0), (sz, 0, 0), (sz, 0, sz), (0, 0, sz)], #front
		[(0, sz, 0), (sz, sz, 0), (sz, sz, sz), (0, sz, sz)], #back
		[(sz, 0, 0), (sz, 0, sz), (sz, sz, sz), (sz, sz, 0)], #right side
		[(0, 0, 0), (0, 0, sz), (0, sz, sz), (0, sz, 0)], # left side
		[(0, 0, sz), (sz, 0, sz), (sz/2, 0, sz + sz/3)], #roof front
		[(0, sz, sz), (sz, sz, sz), (sz/2, sz, sz + sz/3)], # roof back
		[((sz/2, 0, sz + sz/3)), (sz/2, sz, sz + sz/3)], # roof connector
	]

	shift = [-100, -150, 100] # This is so we can move the house in 3d space
	def convert3d(ptlist):
		'''Convert 3d points into 2d image points using the camera matrix

			Use the shift parameter to move the object points around a fixed amount in each dimension
		'''
		img_pt_list = []
		for pt in ptlist:
			vec = np.array([pt[0]+shift[0], pt[1]+shift[1], pt[2]+shift[2], 1])
			newpt = M.dot(vec).T
			newpt = newpt/newpt[2]
			img_pt_list.append((newpt[0].item(), newpt[1].item()))
		return img_pt_list

	img_pts = [convert3d(pts) for pts in house_points]
	def drawable(points):
		'''Converts a set of points into a drawable numpy vector for use with cv2.polylines'''
		points.append(points[0]) # Add the same point so that the entire polygon can be drawn
		pts = np.array(points, np.int32)
		pts.shape = (len(points), 1, 2)
		return pts

	# This draws the house
	img = np.zeros(size, np.uint8)
	for im_pts in img_pts:
		img = cv2.polylines(img, [drawable(im_pts)], False, (255, 255, 255))	
	disp_mat(img, "q")

# Gather our code in a main() function
def main():
	# Read Image
	global image
	image = cv2.imread('ts.jpg',1);
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
	
	problem2()
	problem3()
	problem4()
	# Close the window will exit the program
	cv2.destroyAllWindows()

# Standard boilerplate to call the main() function to begin
# the program.
if __name__ == '__main__':
    main()




