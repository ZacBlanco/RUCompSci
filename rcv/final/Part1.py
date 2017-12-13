'''Structure From Motion - Multiview Stereo

Rutgers University 
Robotics and Computer Vision (Fall 2017) Final Project

Author: Zachary Blanco
Date: 12/14/17

This python module reads in a set of image files of an objects and reconstructs a 3D scene based
on matching keypoints found across the images

We use bundle adjustment and multiview stereo to accomplish this task.
'''
import os
import sys
import subprocess
import logging
import random
import time
import numpy as np
import cv2
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

logger = logging.getLogger('RCV_SFM_Project')

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
    # return downscale(cv2.imread(img, cv2.IMREAD_GRAYSCALE), 800)
    return downscale(cv2.imread(img), 800)

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
    # disp_img(matchim)
    
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


def downscale_and_replaces_imgs(filenames):
    orig_dir = 'Images/orig'
    subprocess.call(['mkdir', orig_dir])
    for f in filenames:
        i1 = read_pipeline(f)
        subprocess.call(['mv', f, orig_dir])
        cv2.imwrite(f.replace(".jpg", "") + '_down.jpg', i1)

def read_bundle_file(filename):
    '''Reads a bundle file and returns the data within a usable structure
    '''
    ptlist = [[], [], []]
    collist = []
    vlist = [] # ? Not sure what a view list is
    camlist = [[], [], []]
    with open(filename, 'r') as f:
        header = f.readline()
        finfo = f.readline()
        cams, pts = finfo.split(' ')
        cams = int(cams)
        pts = int(pts)
        for x in range(cams):
            f.readline() # f, k1, k2
            f.readline() # A 3x3 Rotation Matrix
            f.readline()
            f.readline()
            t = f.readline().split(' ') # a 3x1 translation vector
            # print(t)
            camlist[0].append(float(t[0]))
            camlist[1].append(float(t[1]))
            camlist[2].append(float(t[2]))
        # collist = np.zeros((3, pts), np.int32)
        for x in range(pts):
            p = f.readline()
            p = p.split(' ')
            ptlist[0].append(float(p[0]))
            ptlist[1].append(float(p[1]))
            ptlist[2].append(float(p[2]))
            c = f.readline().split(' ')
            # collist[0, x] = (float(c[0]))
            # collist[1, x] = (float(c[1]))
            # collist[2, x] = (float(c[2]))
            collist.append([float(c[0])/255, float(c[1])/255, float(c[2])/255])
            f.readline() # Read the viewlist (not going to return)
    return (ptlist, collist, camlist)

def load_and_plot(bundle_file):
    data = read_bundle_file(bundle_file)

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    num = len(data[1])
    samp = random.sample(range(num), min(500, num))
    for x in samp:
        ax.scatter(data[0][0][x], data[0][1][x], data[0][2][x], s=40, c=data[1][x], depthshade=True)
        # ax.scatter(data[2][0], data[2][1], data[2][2], marker='^')
    plt.show()

def main(image_files):
    '''Reconstruct an object in 3D using SFM
    
    Our input is a minimum 4 images of the object.

    The output a set of points in 3-dimensional space under bundle/bundle.out
    '''
    # This runs the bundler and uses our sift script to generate the keypoint descriptor files.
    # After this runs, the final output file can be found under bundle/bundle.out
    # export PROJ_HOME=/mnt/c/Users/zacbl/Documents/GitHub/RUCompSci/rcv/final
    # export BUNDLER_PATH=$PROJ_HOME/bundler_sfm


    # MODIFY TO CORRECT LOCATION FOR SYSTEM
    proj_home = '/mnt/c/Users/zacbl/Documents/GitHub/RUCompSci/rcv/final'

    # DOWNLOAD BUNDLER WITHIN THE PROJECT DIRECTORY 
    bundle_path = proj_home + '/bundler_sfm'

    # Set the environment variables
    os.putenv('PROJ_HOME', '/mnt/c/Users/zacbl/Documents/GitHub/RUCompSci/rcv/final')
    os.putenv('BUNDLER_PATH', bundle_path)
    if 'LD_LIBRARY_PATH' in os.environ:
        os.environ['LD_LIBRARY_PATH'] = os.environ['LD_LIBRARY_PATH'] +  bundle_path + '/bin'
    else:
        os.putenv('LD_LIBRARY_PATH', bundle_path + '/bin')
    
    # Run bundle adjustment
    subprocess.call(['bash', 'RunBundler.sh'])
    # Read the bundle.out file to extract the camera parameters and 3d points
    output_file ='bundle/bundle.out'
    load_and_plot(output_file)

if __name__ == "__main__":
    if len(sys.argv) >= 2:
        if sys.argv[1] == 'read':
            output_file ='bundle/bundle.out'
            load_and_plot(sys.argv[2] if len(sys.argv) >= 3 else output_file)
        if sys.argv[1] == 'downscale':
            image_files = get_files_in_dir('./Images', ['png', 'jpg', 'JPG', 'PNG'])
            image_files = downscale_and_replaces_imgs(image_files)
        sys.exit()
    image_files = get_files_in_dir('./Images', ['png', 'jpg', 'JPG', 'PNG'])
    main(image_files)
