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
import numpy as np
import cv2
import utils
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

logger =logging.getLogger('RCV_SFM_Project')


def downscale_and_replaces_imgs(filenames):
    orig_dir = 'Images/orig'
    subprocess.call(['mkdir', orig_dir])
    for f in filenames:
        i1 = utils.read_pipeline(f)
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
        
        for x in range(pts):
            p = f.readline()
            p = p.split(' ')
            ptlist[0].append(float(p[0]))
            ptlist[1].append(float(p[1]))
            ptlist[2].append(float(p[2]))
            c = f.readline().split(' ')
            collist.append((float(c[0]), float(c[1]), float(c[2])))
            f.readline() # Read the viewlist (not going to return)
    return (ptlist, collist, camlist)

def main():
    image_files = utils.get_files_in_dir('./Images', ['png', 'jpg', 'JPG', 'PNG'])
    if len(sys.argv) == 2:
        if sys.argv[1] == 'downscale':
            downscale_and_replaces_imgs(image_files)
        sys.exit()

    


    # for img in image_files:
    #     utils.view_sift_kp(img)
    # Load the images with OpenCV and calibrate via 8-point algorithm
    # Get matching descriptors from SIFT
    
    # This runs the bundler and uses our sift script to generate the keypoint descriptor files.
    # After this runs, the final output file can be found under bundle/bundle.out
    p = subprocess.call(['bash', 'RunBundler.sh'])

    # Read the bundle.out file to extract the camera parameters and 3d points
    output_file ='bundle/bundle.out'
    data = read_bundle_file(output_file)

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(data[0][0], data[0][1], data[0][2], depthshade=True)
    # ax.scatter(data[2][0], data[2][1], data[2][2], marker='^')
    plt.show()

if __name__ == "__main__":
    main()
