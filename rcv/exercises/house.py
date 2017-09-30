import numpy as np
import cv2
import time
from math import cos, sin, radians

def disp(img):
    cv2.imshow("img", img)
    cv2.waitKey(1)

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
    disp(img)

class Camera(object):

    def __init__(self, location, rotation, mext, draw_size=512):
        self.mext = mext
        self.draw_size = draw_size if draw_size > 0 else 512
        self.rotation = rotation
        self.location = location
    
    def move(self, shift):
        self.location = [self.location[i] + shift[i] for i in range(3)]
    def rotate(self, rotate):
        self.rotation = [(self.rotation[i] + rotate[i]) % 360 for i in range(3)]
    def mint(self):
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
            draw(blank, img_pts)


            # update and display
            frame_end = time.perf_counter()
            elapsed = frame_end - frame_start
            left = frame_time - elapsed
            time.sleep(left) if left > 0 else None
            curr_time = time.perf_counter()
            
    def matrix(self):
        return self.mext.dot(self.mint())


def main():
    # Define the house
    sz = 100
    house_points = [
        [(0, 0, 0), (sz, 0, 0), (sz, 0, sz), (0, 0, sz)], #front
		[(0, sz, 0), (sz, sz, 0), (sz, sz, sz), (0, sz, sz)], #back
		[(sz, 0, 0), (sz, 0, sz), (sz, sz, sz), (sz, sz, 0)], #right side
		[(0, 0, 0), (0, 0, sz), (0, sz, sz), (0, sz, 0)], # left side
		[(0, 0, sz), (sz, 0, sz), (sz/2, 0, sz + sz/3)], #roof front
		[(0, sz, sz), (sz, sz, sz), (sz/2, sz, sz + sz/3)], # roof back
		[((sz/2, 0, sz + sz/3)), (sz/2, sz, sz + sz/3)] # roof connector
    ]
    house_points2 = [
        [(0, 0, 0), (sz, 0, 0), (sz, sz, 0), (0, sz, 0)], #front
        [(0, 0, sz), (sz, 0, sz), (sz, sz, sz), (0, sz, sz)], #back
		[(sz, 0, 0), (sz, sz, 0), (sz, sz, sz), (sz, 0, sz)], #right side
		[(0, 0, 0), (0, sz, 0), (0, sz, sz), (0, 0, sz)], # left side
		[(0, sz, 0), (sz, sz, 0), (sz/2, sz + sz/3, 0)], #roof front
		[(0, sz, sz), (sz, sz, sz), (sz/2, sz + sz/3, sz)], #roof back
		[(sz/2, sz + sz/3, 0), (sz/2, sz + sz/3, sz)] # roof connector
    ]
    sx = sy = 1 # problem parameter
    f = 100 # focal length
    ox = oy = 200 # shift
    Mint = np.matrix([[-.707, -.707, 0,   3],
                        [.707, -.707,  0, 0.5],
                        [0,     0,     1,   3]])
    Mext = np.matrix([[f/sx, 0, 256],
                        [0, f/sy, 256],
                        [0,    0,  1]])
    cam = Camera([50, 50, 0], [0, 0, 180], Mext)
    r = 2*sz
    cam.move([-r, -sz, -r])
    cam.rotate([0, 0, 180])
    cam.animate(house_points2, [r/2, 0, 0], [0, 90, 0], 3, 30)
    cam.animate(house_points2, [r/2, 0, r/2], [0, 90, 0], 3, 30)
    cam.animate(house_points2, [-r/2, 0, -r/2], [0, 90, 0], 3, 30)
    cam.animate(house_points2, [-r/2, 0, -r/2], [0, 90, 0], 3, 30)
    cv2.destroyAllWindows()






if __name__ == "__main__":
    main()