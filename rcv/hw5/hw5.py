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

def calc_fundamental(pl, pr):
    '''Calculates the fundamental matrix using 2 sets of corresponding 2D points'''
    if len(pl) is not len(pr):
        raise ValueError("point arrays must be the same length.")

    vals = np.array([])
    for i in range(len(pl)):
        l = pl[i]
        r = pr[i]
        vals = np.append(vals, [l[0]*r[0], l[0]*r[1], l[0], l[1]*r[0], l[1]*r[1], l[1], r[0], r[1], 1])
    vals.shape = (9, 9)
    U, S, V = np.linalg.svd(vals, full_matrices=True)
    return V.T[:,-1]
    
def get_extr_camera_matrix(fundamental):
    '''Calculates the extrinsic M_l matrix from the fundamental matrix, assuming the right camera is the origin'''
    u, s, v = np.linalg.svd(fundamental.T)
    lastcol = v[-1,:] # get row because v = V^T
    epipole = lastcol
    e1 = epipole[0]
    e2 = epipole[1]
    e3 = epipole[2]
    ex = np.array([
        [0, -e3, e2],
        [e3, 0, -e1],
        [-e2, e1, 0]
    ])
    ml = np.zeros((3, 4))
    ml[:, :3] = np.array(ex.dot(fundamental))
    ml[:,-1] = epipole
    return ml

def problem1():
    Mint = np.matrix([[f/sx, 0, 256],
                    [0, f/sy, 256],
                    [0,    0,  1]])
    
    poly = [(0, 0, 0), (50, 0, 100), (150, 0, 100), (100, 0, 0), (50, 100, 50), (100, 100, 150), (200, 100, 150), (150, 100, 0), (100, 200, 100)]
    # print(poly[0:4])
    polygon_faces = [
        poly[0:4], # polygon base
        poly[4:8], # poly base 2
        [poly[0], poly[4]], # vert connector 1
        [poly[1], poly[5]], # vert connector 2
        [poly[2], poly[6]], # vert connector 3
        [poly[3], poly[7]], # vert connector 4
        [poly[4], poly[8]], # tip connector 1
        [poly[5], poly[8]], # tip connector 2
        [poly[6], poly[8]], # tip connector 3
        [poly[7], poly[8]], # tip connector 4
    ]
    cam = Camera([50, 50, 0], [0, 0, 180], Mint)
    cam.move([-150, -100, -200])
    cam.rotate([0, 0, 180])
    unique_pts = [poly]
    plane_pts = [ cam.convert3d(pt) for pt in unique_pts]
    # print(plane_pts)
    # print("Press 'n' to view next camera angle")
    # cam.disp(polygon_faces, key='n')
    cam.move([75, 0, 25])
    cam.rotate([0, 20, 0])
    plane_pts.append([cam.convert3d(pt) for pt in unique_pts][0])
    # print("Press 'n' to close")
    # cam.disp(polygon_faces, key='n')
    fund = calc_fundamental(plane_pts[0], plane_pts[1])
    fund.shape = (3, 3)
    fund_cv = cv2.findFundamentalMat(np.array(plane_pts[0]), np.array(plane_pts[1]), cv2.FM_8POINT)[0]
    print("NORMAL 8 POINT FUNDAMENTAL")
    print(fund)
    print("OPENCV FUNDAMENTAL ALGORITHM")
    print(fund_cv)
    return (fund, cam.mint, plane_pts)

def problem2(fund, Kl, Kr):
    E = Kr.T.dot(fund).dot(Kl) # Calculation of Essential Matrix
    print("Essential Matrix".upper())
    print(E)
    return E

def problem3(fundamental, essential, K, img_pts):

    # First Camera left and right views
    poly = [(0, 0, 0), (50, 0, 100), (150, 0, 100), (100, 0, 0), (50, 100, 50), (100, 100, 150), (200, 100, 150), (150, 100, 0), (100, 200, 100)]
    # print(poly[0:4])
    polygon_faces = [
        poly[0:4], # polygon base
        poly[4:8], # poly base 2
        [poly[0], poly[4]], # vert connector 1
        [poly[1], poly[5]], # vert connector 2
        [poly[2], poly[6]], # vert connector 3
        [poly[3], poly[7]], # vert connector 4
        [poly[4], poly[8]], # tip connector 1
        [poly[5], poly[8]], # tip connector 2
        [poly[6], poly[8]], # tip connector 3
        [poly[7], poly[8]], # tip connector 4
    ]
    cam = Camera([50, 50, 0], [0, 0, 180], Mint)
    cam.move([-150, -100, -200])
    cam.rotate([0, 0, 180])
    cam.disp(polygon_faces, key='n')
    cam.move([75, 0, 25])
    cam.rotate([0, 20, 0])
    cam.disp(polygon_faces, key='n')

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
    u, s, v = np.linalg.svd(essential, full_matrices=True)
    # Ml = get_extr_camera_matrix(fundamental)
    # print("LEFT CAMERA MATRIX")
    # Mleft = K.dot(Ml)
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
    for _s in s:
        for _r in r:
            Mleft = np.zeros((3, 4))
            Mleft[:,:3] = _r 
            Mleft[:,3] = np.array([_s[2, 1],_s[0, 2], -_s[0, 1] ])
            Mleft = K.dot(Mleft)
            recon = cv2.triangulatePoints(Mleft, Mright, np.array(img_pts[0]).T, np.array(img_pts[1]).T)
            if np.amin(s) > 0:
                found = True
            if found:
                break
        if found:
            break
    
    cam = Camera([50, 50, 0], [0, 0, 180], Mint)
    cam.move([-150, -100, -200])
    cam.rotate([0, 0, 180])
    poly = homogeneous_to_3d(recon, scale=1000)
    polygon_faces = [
        poly[0:4], # polygon base
        poly[4:8], # poly base 2
        [poly[0], poly[4]], # vert connector 1
        [poly[1], poly[5]], # vert connector 2
        [poly[2], poly[6]], # vert connector 3
        [poly[3], poly[7]], # vert connector 4
        [poly[4], poly[8]], # tip connector 1
        [poly[5], poly[8]], # tip connector 2
        [poly[6], poly[8]], # tip connector 3
        [poly[7], poly[8]], # tip connector 4
    ]
    cam.disp(polygon_faces, key='n')

def homogeneous_to_3d(pts, scale=100):
    pts = pts.T
    p = []
    for i in range(pts.shape[0]):
        row = pts[i,:]
        p.append((scale*row[0]/row[3], scale*row[1]/row[3], scale*row[2]/row[3]))
    return p

def problem4():
    Mint = np.matrix([[f/sx, 0, 256],
                    [0, f/sy, 256],
                    [0,    0,  1]])
    
    poly = [(0, 0, 0), (50, 0, 100), (150, 0, 100), (100, 0, 0), (50, 100, 50), (100, 100, 150), (200, 100, 150), (150, 100, 0), (100, 200, 100)]
    polygon_faces = [
        poly[0:4], # polygon base
        poly[4:8], # poly base 2
        [poly[0], poly[4]], # vert connector 1
        [poly[1], poly[5]], # vert connector 2
        [poly[2], poly[6]], # vert connector 3
        [poly[3], poly[7]], # vert connector 4
        [poly[4], poly[8]], # tip connector 1
        [poly[5], poly[8]], # tip connector 2
        [poly[6], poly[8]], # tip connector 3
        [poly[7], poly[8]], # tip connector 4
    ]
    cam = Camera([50, 50, 0], [0, 0, 180], Mint)
    cam.move([-150, -100, -200])
    cam.rotate([0, 0, 180])
    unique_pts = [poly]
    plane_pts = [ cam.convert3d(pt) for pt in unique_pts]
    # print(plane_pts)
    print("Press 'n' to view next camera angle")
    cam.disp(polygon_faces, key='n')
    Ml = cam.matrix()
    cam.move([75, 0, 25])
    cam.rotate([0, 20, 0])
    plane_pts.append([cam.convert3d(pt) for pt in unique_pts][0])
    print("Press 'n' to close")
    cam.disp(polygon_faces, key='n')
    Mr = cam.matrix()
    recon = cv2.triangulatePoints(Ml, Mr, np.array(plane_pts[0]).T, np.array(plane_pts[1]).T)
    poly = homogeneous_to_3d(recon, scale=5)
    polygon_faces = [
        poly[0:4], # polygon base
        poly[4:8], # poly base 2
        [poly[0], poly[4]], # vert connector 1
        [poly[1], poly[5]], # vert connector 2
        [poly[2], poly[6]], # vert connector 3
        [poly[3], poly[7]], # vert connector 4
        [poly[4], poly[8]], # tip connector 1
        [poly[5], poly[8]], # tip connector 2
        [poly[6], poly[8]], # tip connector 3
        [poly[7], poly[8]], # tip connector 4
    ]
    cam.move([0, 0, -1000])
    cam.rotate([0, 0, 0])
    cam.disp(polygon_faces, key='n')

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
    return K, tvecs, rvecs

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


def problem5():
    '''Checkerboard calibration'''
    K, tv , rv= calibrate(20)
    print("Checkerboard Calibration K")
    print(K)
    # print(tv)
    rv = [ cv2.Rodrigues(r)[0] for r in rv]
    cTw = [np.zeros((3, 4)) for x in range(len(rv))]
    for m in range(len(cTw)):
        cTw[m][:3, :3] = rv[m]
        cTw[m][:,3] = tv[m].T
    print("cTw Sample")
    print(cTw[0])
    import matplotlib.pyplot as plt
    from mpl_toolkits.mplot3d import Axes3D
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    
    x = [ np.asscalar(vec[0]) for vec in tv ]
    y = [ np.asscalar(vec[1]) for vec in tv ]
    z = [ np.asscalar(vec[2]) for vec in tv ]
    ax.scatter(x, y, z, s=len(x), c='r')
    plt.show()
    


def main():
    fund, K, points = problem1()
    E = problem2(fund, K, K)
    problem3(fund, E, K, points)
    problem4()
    problem5()

if __name__ == "__main__":
    main()


    