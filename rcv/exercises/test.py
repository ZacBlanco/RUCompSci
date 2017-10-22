import sys
import cv2
import numpy as np
from matplotlib import pyplot as plt

def show_image(filename):
    img = cv2.imread(filename, 1)
    cv2.waitKey(0)
    cv2.imshow("img", image)
    cv2.destroyAllWindows()

def disp_mat(data):
    cv2.imshow('img', data/255.0)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

def svd_compress(img_matrix, dims):
    U, S, V = np.linalg.svd(img_matrix, True)
    U = U[,:dims]
    S = np.diag(S)[:dims,:dims]
    V = V[:dims,]
    return U.dot(S).dot(V)


def warp(image, refPts, mapPts):
    def warp(image, orig, mapped):
	'''Warps the image via estimating the 2D homography.

	User must select the points in the following order to warp the billboard with the same
	perspective.

	- Top Left
	- Top Right
	- Bottom Left
	- Bottom Right
	'''
	# Estimate the homography
	Amat = []
	for i in range(len(orig)):
		rp = orig[i]
		hp = mapped[i]
		Amat.append([-hp[0], -hp[1], -1, 0, 0, 0, rp[0]*hp[0], rp[0]*hp[1], rp[0]])
		Amat.append([0, 0, 0, -hp[0], -hp[1], -1, rp[1]*hp[0], hp[1]*rp[1], rp[1]])
	Amat = np.matrix(Amat)
	# Get the homography from the null space of A (last col of V)
	u, s, v = np.linalg.svd(Amat)
	v = v.H # The V returned is really V.T
	hom = v[:,-1] # equivalent to 
	hom.shape = (3, 3) # Reshape back into the 3x3

	# numpy arrays for mappings
	mapx = np.zeros(image.shape[:2], np.float32)
	mapy = np.zeros(image.shape[:2], np.float32)

	# Calculate the mapping using the homography matrix
	for y in range(image.shape[0]): # rows
		for x in range(image.shape[1]): # cols
			vec = np.array([x, y, 1]).T
			pts = hom.dot(vec).T
			pts = pts/pts[2]
			mapx[y, x] = pts[0]
			mapy[y, x] = pts[1]
	
	# performs the remapping and display the new image
	I2 = np.zeros(image.shape, np.float32)
	I2 = cv2.remap(image, mapx, mapy, 1)
    return I2