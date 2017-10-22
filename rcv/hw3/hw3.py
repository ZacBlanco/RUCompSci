import numpy as np
import cv2
import time
from math import cos, sin, radians

sz = 100
r = 2*sz
house_points = [
    [(0, 0, 0), (sz, 0, 0), (sz, sz, 0), (0, sz, 0)], #front
    [(0, 0, sz), (sz, 0, sz), (sz, sz, sz), (0, sz, sz)], #back
    [(sz, 0, 0), (sz, sz, 0), (sz, sz, sz), (sz, 0, sz)], #right side
    [(0, 0, 0), (0, sz, 0), (0, sz, sz), (0, 0, sz)], # left side
    [(0, sz, 0), (sz, sz, 0), (sz/2, sz + sz/3, 0)], #roof front
    [(0, sz, sz), (sz, sz, sz), (sz/2, sz + sz/3, sz)], #roof back
    [(sz/2, sz + sz/3, 0), (sz/2, sz + sz/3, sz)] # roof connector
]
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

def drawmyobject(points, cam):
    cam.disp(points)


def draw(blank_image, img_points):
    def drawable(points):
        '''Converts a set of points into a drawable numpy vector for use with cv2.polylines'''
        points.append(points[0]) # Add the same point so that the entire polygon can be drawn
        pts = np.array(points, np.int32)
        pts.shape = (len(points), 1, 2)
        return pts

	# This draws the house
    img = blank_image
    for im_pts in img_points:
        img = cv2.polylines(img, [drawable(im_pts)], False, (255, 255, 255))	
    return img

class Camera(object):

    def __init__(self, location, rotation, mint, draw_size=512):
        self.mint = mint
        self.draw_size = draw_size if draw_size > 0 else 512
        self.rotation = rotation
        self.location = location
    
    def move(self, shift):
        self.location = [self.location[i] + shift[i] for i in range(3)]
    def rotate(self, rotate):
        self.rotation = [(self.rotation[i] + rotate[i]) % 360 for i in range(3)]
    def mext(self):
        m = np.zeros((3, 4), np.float64)
        m[:,:3] = self.rotationMatrix()
        m[:,3] = np.array(self.location)
        return m
    def rotationMatrix(self):
        zmat = np.matrix([
            [cos(radians(self.rotation[2])), -sin(radians(self.rotation[2])), 0],
            [sin(radians(self.rotation[2])), cos(radians(self.rotation[2])), 0],
            [0, 0, 1]
        ])

        ymat = np.matrix([
            [cos(radians(self.rotation[1])), 0,  sin(radians(self.rotation[1]))],
            [0, 1, 0],
            [-sin(radians(self.rotation[1])), 0,  cos(radians(self.rotation[1]))]
        ])

        xmat = np.matrix([
            [1, 0, 0],
            [0, cos(radians(self.rotation[0])),  -sin(radians(self.rotation[0]))],
            [0, sin(radians(self.rotation[0])), cos(radians(self.rotation[0]))]
        ])
        return zmat.dot(ymat).dot(xmat)
    def convert3d(self, ptlist):
        '''Convert 3d points into 2d image points using the camera matrix
            Use the shift parameter to move the object points around a fixed amount in each dimension
        '''
        img_pt_list = []
        camera_matrix = self.matrix()
        for pt in ptlist:
            vec = np.array([pt[0], pt[1], pt[2], 1])
            newpt = camera_matrix.dot(vec).T
            newpt = newpt/newpt[2]
            img_pt_list.append((newpt[0].item(), newpt[1].item()))
        return img_pt_list

    def disp(self, points, key="c"):
        img_pts = [self.convert3d(pts) for pts in points]
        blank = np.zeros((self.draw_size, self.draw_size), np.uint8)
        disp_key(draw(blank, img_pts), key)

    def animate(self, points, shift, rotate, anim_time, max_fps):
        '''Animate the 3d points using the camera movement

        points: The 3d points to display
        shift: The total amount for the camera to move
        rotate: A length 3 list indicating the degrees to rotate
        time: Total time for the animation (seconds)
        max_fps: The maximum fps for the scene to render at
        '''
        
        frame_time = 1/max_fps
        dshift = [x / (max_fps*anim_time) for x in shift]
        drotate = [x / (max_fps*anim_time) for x in rotate]
        start = time.perf_counter()
        curr_time = start
        while curr_time - start < anim_time:
            frame_start = time.perf_counter()
            # update and display
            self.move(dshift)
            self.rotate(drotate)
            img_pts = [self.convert3d(pts) for pts in points]
            blank = np.zeros((self.draw_size, self.draw_size), np.uint8)
            disp(draw(blank, img_pts))


            # update and display
            frame_end = time.perf_counter()
            elapsed = frame_end - frame_start
            left = frame_time - elapsed
            time.sleep(left) if left > 0 else None
            curr_time = time.perf_counter()
            
    def matrix(self):
        return self.mint.dot(self.mext())


def problem1():
    cam = Camera([50, 50, 0], [0, 0, 180], Mint)
    cam.move([-r, -sz, -r])
    cam.rotate([0, 0, 180])
    drawmyobject(house_points, cam)

def problem2():
    cam = Camera([50, 50, 0], [0, 0, 180], Mint)
    cam.move([-r, -sz, -r])
    cam.rotate([0, 0, 180])
    cam.animate(house_points, [r/2, 0, 0], [0, 90, 0], 3, 30)
    cam.animate(house_points, [r/2, 0, r/2], [0, 90, 0], 3, 30)
    cam.animate(house_points, [-r/2, 0, -r/2], [0, 90, 0], 3, 30)
    cam.animate(house_points, [-r/2, 0, -r/2], [0, 90, 0], 3, 30)
    # cam.animate(house_points, [0, 0, 0], [0, 0, 180], 3, 30)
    # cam.animate(house_points, [0, 0, 0], [0, 0, -180], 3, 30)
    cam.disp(house_points, key="n")
    cam.animate(house_points, [2*r/3, 0, r/3], [70, 20, 0], 3, 30)
    cam.animate(house_points, [0, 0, -r/2], [0, 0, 0], 10, 30)
    cam.disp(house_points, key="n")

def get_n_photos(n):
    ''' return up to n photo filenames
    '''
    import os
    from os import listdir
    from os.path import isfile, join
    photos = [ join("./Photos", f) for f in listdir("./Photos") if isfile(join("./Photos", f))]
    if n < len(photos):
        return photos[:n]
    else:
        return photos

def calibrate(n):
    '''calibrate a camera using up to n images'''
    width = 7
    height = 9
    # board_height = 90 # mm
    # board_height = 120 # mm
    # All rows and only xy coords
    worldcoords = np.zeros((width*height, 3), np.float32)    
    worldcoords[:, :2] = np.mgrid[0:height,0:width].T.reshape(-1, 2)
    corner_list = []
    worldpoints = []
    photo = None
    for photo in get_n_photos(n):
        a = cv2.imread(photo)
        a = cv2.cvtColor(a, cv2.COLOR_BGR2GRAY)
        ret, corners = cv2.findChessboardCorners(a, (height, width))
        # refined_corners = cv2.cornerSubPix(a, corners, (10, 10), (-1, -1), )
        corner_list.append(corners)
        worldpoints.append(worldcoords)
        photo = a
        # im = cv2.drawChessboardCorners(a, (7, 9), corners, ret)
        # cv2.namedWindow("i", cv2.WINDOW_NORMAL)
        # cv2.imshow("i", im)
        # cv2.resizeWindow("i", 1000, 1000)
        # cv2.waitKey(0)
        # cv2.destroyAllWindows()

    # We've added everything and retrieved all of the corners
    ret, K, distCoeffs, rvecs, tvecs = cv2.calibrateCamera(worldpoints, corner_list, photo.shape[::-1], None, None)
    return K

def problem4():
    for i in range(5, 31, 5):
        print("{} photo calibration".format(i))
        print(calibrate(i))
   
    
   
    

def main():
    problem1()
    problem2()
    # uncomment this line if there is a directory labeled "Photos" which contains all images used to calibrate the camera
    # problem4()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()