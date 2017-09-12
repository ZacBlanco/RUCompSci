#! /usr/bin/python

import numpy as np
import cv2
from matplotlib import pyplot as plt

# import modules used here
import sys

def problem2():
    a = np.matrix([[4.29, 5.30, 1.33], [2.2, 10.1, 4.8], [5.51, -8.24, -6.62]])
    r = np.linalg.matrix_rank(a)
    u, s, v = np.linalg.svd(a, full_matrices=1, compute_uv=1)
    # print(u)
    # print("Len U = {}".format(np.shape(u)[0]))
    # print("Rank: {}".format(r))

    # Compares the rank of A and size of U columns to determine independence
    if r == np.shape(u)[1]:
        return False # If the rank and u are equal then more than 2 cols (in this problem) are independent
    # Another way:
    # By solving the matrix for the 0 vector we can determine that if there is more than one solution
    # which exists other than [0, 0, 0] then the columns are linearly indepedent.
    val = np.linalg.solve(a, np.array([0, 0, 0]))
    return False if val.all() == np.array([0, 0, 0]).all() else True

def problem3():
    pts = np.matrix([
        [0, 1],
        [1, 3.2],
        [1.9, 5],
        [3, 7.2],
        [3.9, 9.3],
        [5, 11.1]
    ])
    # A = pts
    # print(pts[:,0])
    A = np.concatenate((pts[:,0].T, np.matrix([1, 1, 1, 1, 1, 1])), axis=0).T
    b = pts[:,1]
    # print(b)
    # print(A)
    sol = np.linalg.inv(A.T*A)*A.T*b
    # Create a graph showing the regression line.
    plt.figure()
    
    for x in range(80):
        plt.plot(x/10, sol[0]*x/10 + sol[1], 'ro', linewidth=0.2)

    for x in range(np.shape(pts)[0]):
        plt.plot(pts[:,0], pts[:,1], 'b.')

    plt.show()
    print("q parameters for least squares approximation: {}".format(sol))


# Gather out code in a main() function
def main():
    print('Hello there {}'.format(sys.argv[1]))
    # Command line args are in sys.argv[1], sys.argv[2]
    # sys.argv[0] is the script name itself and can be ignored
    img = cv2.imread('testimg.jpg',1)
    # Display with matplotlib
    plt.imshow(img, interpolation='bicubic')
    plt.xticks([])
    plt.yticks([]) # to hide tick values on X and Y axis
    plt.show()
    print("The matrix has 2 linearly independent columns: {}".format(problem2()))
    problem3()
    # Close the window will exit the program
    cv2.destroyAllWindows()
    


if __name__ == "__main__":
    main()
