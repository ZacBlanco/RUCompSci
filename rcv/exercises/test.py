import sys
import cv2
import numpy as np
from matplotlib import pyplot as plt

def show_image(filename):
    img = cv2.imread(filename, 1)
    plt.figure()
    plt.imshow(img, interpolation='bicubic')
    plt.xticks([])
    plt.yticks([])
    plt.show()
    cv2.destroyAllWindows()

def disp_mat(data):
    cv2.imshow('img', data/255.0)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

def resize_img(img_matrix, dims):
    U, S, V = np.linalg.svd(img_matrix, True)
    U = U[:,:dims]
    S = np.diag(S)[:dims,:dims]
    V = V[:dims,]
    return U.dot(S).dot(V)

def main():
    # show_image('baby-monkey.jpg')
    d = cv2.imread('baby-monkey.jpg', False)
    U, S, V = np.linalg.svd(d)
    disp_mat(d)
    i = resize_img(d, 20)
    disp_mat(i)
    disp_mat(d-i)
    i = resize_img(d, 40)
    disp_mat(i)
    i = resize_img(d, 80)
    disp_mat(i)
    i = resize_img(d, 200)
    disp_mat(i)





if __name__ == "__main__":
    main()