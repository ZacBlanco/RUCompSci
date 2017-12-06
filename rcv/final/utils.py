'''A set of utility functions which support the RCV final project.
'''
import os
import unittest
import cv2

def get_files_in_dir(directory, extension=None):
    '''Returns all of the immediate files within the specified directory
    '''
    dirfiles = None
    for r, d, f in os.walk(directory):
        dirfiles = f
        break

    if extension is not None:
        if type(extension) == str:
            dirfiles = filter(lambda x: x.endswith(extension), dirfiles)
        elif type(extension) == list:
            for ext in extension:
                if type(ext) == str:
                    dirfiles = filter(lambda x: x.endswith(ext), dirfiles)
    return list(dirfiles)

# def read_images


def disp_key(data, key='n', width=None, height=None):
    '''Display an image using OpenCV, close with a keypress of specified key

    Arguments:
        data (numpy.ndarray): The image to display
        key (str): A single character key used to close the image window. Defaults to 'n'
        width (int): The width of the window
        height (int): The height of the window

    '''
    cv2.namedWindow("img", cv2.WINDOW_NORMAL)
    cv2.imshow('img', data)
    if width != None and height != None:
        cv2.resizeWindow('img', int(width), int(height))
    while True:
        k = cv2.waitKey(50) & 0xFF
        if k == ord(key):
            break
    cv2.destroyAllWindows()


class utilsTest(unittest.TestCase):

    def test_files_in_dir(self):
        self.assertTrue(len(get_files_in_dir('.')) == 4, "Should only be 4 files")
        self.assertTrue(len(get_files_in_dir('.', extension='.py')) == 3, "Should only be 2 files")
        


if __name__ == "__main__":
    unittest.main()
