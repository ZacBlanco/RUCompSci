import numpy as np
import cv2
import time
from math import cos, sin, radians
from matplotlib import pyplot as plt

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

def problem1(imleft, imright):
	imgs = [imleft, imright]
	sift = cv2.xfeatures2d.SIFT_create()
	grays = [cv2.imread(x, cv2.IMREAD_GRAYSCALE) for x in imgs]
	# Left = 0
	# Right = 1
	kp_des = [ sift.detectAndCompute(x, None) for x in grays]
	bf = cv2.BFMatcher()
	matches = bf.knnMatch(kp_des[0][1], kp_des[1][1], k=2)
	good = get_good_matches(matches)
	good = [ [m] for m in good]
	print("Number of good keypoints: {}".format(len(good)))
	out = None
	im3 = cv2.drawMatchesKnn(grays[0], kp_des[0][0], grays[1], kp_des[1][0], good, out) 
	plt.imshow(im3)
	plt.show()
	return (kp_des, matches, good)

def get_good_matches(matches, ratio=0.7):
	'''Return a list of the matches which best the next'''
	good = []
	for m, n in matches:
		if m.distance < ratio*n.distance:
			good.append(m)
	return good

def problem2(pl, pr):
	pts1 = np.array(pl)
	pts2 = np.array(pr)
	K = np.array([
		[3086.6346, 0, 1648.85125],
		[0, 3290.12643, 1932.62623],
		[0, 0, 1] 
	]) # Derived in Homework 5
	F, o = cv2.findFundamentalMat(pts1, pts2)
	E = K.T.dot(F).dot(K)
	print("===== Intrinsic Matrix =====")
	print(K)
	print("===== Fundamental Matrix =====")
	print(F)
	print("===== Essential Matrix =====")
	print(E)
	return (K, F, E)

def problem3(K, F, E, pl, pr):
	'''Calculate the Camera Matrix from K/E'''
	w = np.array([
		[0, -1, 0],
		[1,  0, 0],
		[0,  0, 1]
	])
	z = np.array([
		[ 0, 1, 0],
		[-1, 0, 0],
		[ 0, 0, 0]
	])
	u, s, v = np.linalg.svd(E, full_matrices=True)
	Mright = np.zeros((3, 4))
	Mright[:3, :3] = np.eye(3)
	Mright = K.dot(Mright)
	s1 = -u.dot(z).dot(u.T)
	s2 = u.dot(z).dot(u.T)
	r1 = u.dot(w.T).dot(v.T)
	r2 = u.dot(w).dot(v.T)
	s = [s1, s2]
	r = [r1, r2]
	found = False
	recon = None
	Mleft = None
	lefts = []
	num_negs = []
	for _s in s:
		for _r in r:
			Mleft = np.zeros((3, 4))
			Mleft[:,:3] = _r 
			Mleft[:,3] = np.array([_s[2, 1],_s[0, 2], -_s[0, 1] ])
			Mleft = K.dot(Mleft)
			recon = cv2.triangulatePoints(Mleft, Mright, np.array(pl).T, np.array(pr).T)
			pts = homogeneous_to_3d(recon, keep_numpy=True)
			negs = (pts[:,2] < 0).sum()
			num_negs.append(negs)
			lefts.append(Mleft)
			if np.amin(pts[:,2]) > 0:
				found = True
			if found:
				break
		if found:
			break
	if not found:
		print("Could not compute a positive Z value for all matching keypoints")
		print("We're going to use the matrix with the least number of negative Z values")
		print("Number of keypoints {}".format(len(pts)))
		print("Number of negative Z axis reconstructed matches: {}".format(min(num_negs)))
		ind = num_negs.index(min(num_negs))
		Mleft = lefts[ind]

	print("====== Essential Used to Calculate Camera Matrices ======")
	print(E)
	print("====== Left Camera Matrix ======")
	print(Mleft)
	print("====== Right Camera Matrix ======")
	print(Mright)

	return (Mleft, Mright, np.array(pl).T, np.array(pr).T)

def problem4(ml, mr, pl, pr):
	recon = cv2.triangulatePoints(ml, mr, pl, pr)
	td = homogeneous_to_3d(recon)
	import matplotlib.pyplot as plt
	from mpl_toolkits.mplot3d import Axes3D
	# fig = plt.figure()
	# ax1 = fig.add_subplot(111)
	# ax1.scatter(pl.T[:,0], pl.T[:,1], c='r')
	# ax1.scatter(pr.T[:,0], pr.T[:,1], c='b')
	# plt.show()
	fig = plt.figure()
	ax = fig.add_subplot(111, projection='3d')
	x = []
	y = []
	z = []
	for pt in td:
		x.append( pt[0] )
		y.append( pt[1] )
		z.append( pt[2] )
	ax.scatter(x, y, z, c='r')
	plt.show()

def homogeneous_to_3d(pts, scale=1, keep_numpy=False):
	pts = pts.T
	if keep_numpy:
		p = np.zeros((pts.shape[0], 3))
	else:
		p = []

	for i in range(pts.shape[0]):
		row = pts[i,:]
		x = scale*row[0]/row[3]
		y = scale*row[1]/row[3]
		z = scale*row[2]/row[3]	
		if not keep_numpy:
			p.append((x, y, z))
		else:
			p[i,:] = (x, y, z)
	return p

def main():
	kp, matches, good = problem1("left.jpg", "right.jpg")
	good = []
	pr = []
	pl = []
	good = get_good_matches(matches)
	for m in good:
		p1 = kp[0][0][m.queryIdx].pt # Left img point
		p2 = kp[1][0][m.trainIdx].pt # right img point
		pl.append(p1)
		pr.append(p2)
	
	K, F, E = problem2(pl, pr)
	ml, mr, pl, pr = problem3(K, F, E, pr, pl)
	problem4(ml, mr, pl, pr)

if __name__ == "__main__":
	main()