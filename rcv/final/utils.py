'''A set of utility functions which support the RCV final project.
'''
import os
import logging
import time
import unittest
import cv2
import numpy as np
# Zac
# SPN: 923588

logger = logging.getLogger('RCV_UTILS')

def get_files_in_dir(directory, extension=None):
    '''Returns all of the immediate files within the specified directory

    Arguments:
        directory (str): The directory to search in
        extension (str or list): A single extension or list of extensions to match.
                                 If None, all files are returned.

    Returns:
        list: The list of files matching the extensions within the directory.
    '''
    dirfiles = None
    for info in os.walk(directory):
        dirfiles = info[2]
        break

    if extension is not None:
        if isinstance(extension, str):
            dirfiles = filter(lambda x: x.endswith(extension), dirfiles)
        elif isinstance(extension, list):
            def hasext(l):
                for k in extension:
                    if l.endswith(k):
                        return True
                return False
                if isinstance(ext, str):
                    dirfiles = filter(lambda x: hasext(x), dirfiles)
    return [ directory + '/' + x for x in list(dirfiles)]

def downscale(image, maxdim=600):
    '''downscale the images to a resolution where the lowest dimension is at most maxdim

    - Uses cv2.pyrDown() which includes a gaussian blurring

    Arguments:
        image (numpy.ndarray): Image to downscale
        maxdim (int): The maximum value for the smaller dimension to be constrained by
    '''
    img = image
    while img.shape[0] > maxdim and img.shape[1] > maxdim:
        img = cv2.pyrDown(img)
    
    return img


def disp_img(data, key='n', width=None, height=None, timeout=False, timeout_length=10):
    '''Display an image using OpenCV, close with a keypress of specified key

    Arguments:
        data (numpy.ndarray): The image to display
        key (str): A single character key used to close the image window. Defaults to 'n'
        width (int): The width of the window
        height (int): The height of the window
        timeout (bool): Close the window after a specified timeout length
        timeout_length (float): Number of seconds to wait before closing the window

    '''
    cv2.namedWindow("img", cv2.WINDOW_NORMAL)
    cv2.imshow('img', data)
    if width != None and height != None:
        cv2.resizeWindow('img', int(width), int(height))
    start = time.time()
    while True:
        end = time.time()
        k = cv2.waitKey(50) & 0xFF
        if k == ord(key):
            break
        if end-start > timeout_length:
            break
    cv2.destroyAllWindows()

# The Intrinsic Camera Matrix (From my phone)
K = np.array([
        [3086.6346, 0, 1648.85125],
        [0, 3290.12643, 1932.62623],
        [0, 0, 1] 
    ]) # Derived in Homework 5

def filter_sift_matches(matches, ratio=0.7):
    '''Takes a set of SIFT matches and filters out ones which are most likely to be "good" matches
    '''
    good = []
    for m, n in matches:
        if m.distance < ratio*n.distance:
            good.append(m)
    return good

def read_pipeline(img):
    return downscale(cv2.imread(img, cv2.IMREAD_GRAYSCALE), 800)

def view_sift_kp(img):
    gray = read_pipeline(img)
    
    sift = cv2.xfeatures2d.SIFT_create()
    kp = sift.detect(gray, None)
    kpimg = cv2.drawKeypoints(gray,kp, None)
    disp_img(kpimg, width=800, height=800)

def get_sift_points(imleft, imright):
    '''Gets the set of SIFT descriptors and keypoints from a left and right image
    '''
    imgs = [imleft, imright]
    grays = [ read_pipeline(x) for x in imgs]

    
    sift = cv2.xfeatures2d.SIFT_create()
    kp_des = [sift.detectAndCompute(x, None) for x in grays]

    # Right image data at index 1
    bf = cv2.BFMatcher()
    matches = bf.knnMatch(kp_des[0][1], kp_des[1][1], k=2) # Match 2 best points
    good = filter_sift_matches(matches)
    print("Number of matches: {}".format(len(good)))

    # good = [[m] for m in good]        
    # out = None
    # matchim = cv2.drawMatchesKnn(grays[0], kp_des[0][0], grays[1], kp_des[1][0], good, out)
    # utils.disp_img(matchim)
    
    return (kp_des, good, imleft, imright)

def calculate_essential(left, right):
    pl = np.array(left)
    pr = np.array(right)
    F = None
    for x in [cv2.FM_RANSAC, cv2.FM_8POINT, cv2.FM_7POINT, cv2.FM_LMEDS]:
        F, o = cv2.findFundamentalMat(pl, pr, x)
        if F is not None:
            break
    if F is None:
        logger.warning("Could not calculate Fundamental matrix")
        return (None, None)
    if F.shape != (3, 3):
        F = F[:3, :]
    E = K.T.dot(F).dot(K)
    return F, E

def calculate_camera_matrix(E, pl, pr):
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
    return (Mleft, Mright)


class utilsTest(unittest.TestCase):

    def test_files_in_dir(self):
        self.assertTrue(len(get_files_in_dir('.')) == 4, "Should only be 4 files")
        self.assertTrue(len(get_files_in_dir('.', extension='.py')) == 3, "Should only be 2 files")

    def test_dispkey(self):
        import numpy as np
        a = np.array([[1, 2, 3],
                      [1, 2, 3],
                      [1, 2, 3]])
        disp_img(a)        


if __name__ == "__main__":
    unittest.main()
