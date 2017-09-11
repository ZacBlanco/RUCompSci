#! /usr/bin/python

import numpy as np
import cv2
from matplotlib import pyplot as plt

# import modules used here
import sys

# Gather out code in a main() function
def main():
    print('Hello there {}'.format(sys.argv[1]))
    # Command line args are in sys.argv[1], sys.argv[2]
    # sys.argv[0] is the script name itself and can be ignored
    img = cv2.imread('testimg.jpg',1)
    # Display with matplotlib
    plt.imshow(img, interpolation = 'bicubic')
    plt.xticks([]), plt.yticks([]) # to hide tick values on X and Y axis
    plt.show()
    # Close the window will exit the program
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
