#! /usr/bin/env python3

import sys
import io
import os
import math
import cv2 # requires opencv-contrib-python


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("ERROR")
        print("Must provide at least 2 arguments {INPUT_FILE} {OUTPUT_FILE}")
        sys.exit(1)
    input = sys.argv[1]
    output = sys.argv[2]
    sift = cv2.xfeatures2d.SIFT_create()
    kps, des= sift.detectAndCompute(cv2.imread(input, cv2.IMREAD_GRAYSCALE), None)
    kp_len = len(kps)
    torad = lambda x: math.pi/180*(x if x < 180 else -1*(x-180))
    with open(output, 'w') as f:
        f.write("{} 128\n".format(len(kps)))
        for x in range(len(kps)):
            k = kps[x]
            d = []
            d.append(des[x][:20].tolist())
            d.append(des[x][20:40].tolist())
            d.append(des[x][40:60].tolist())
            d.append(des[x][60:80].tolist())
            d.append(des[x][80:100].tolist())
            d.append(des[x][100:120].tolist())
            d.append(des[x][100:120].tolist())
            f.write(" ".join([str(round(k.pt[0], 3)), str(round(k.pt[1], 3)), str(round(k.size, 3)), str(round(torad(k.angle), 3))]))
            f.write('\n')
            for row in d:
                [f.write(' ' + str(int(h))) for h in row]
                f.write('\n')
            
            
    # for i in range(10):
    #     k = kp[i]

    #     print(round(k.pt[0], 3), round(k.pt[1], 3), round(k.size, 3), round(torad(k.angle), 3))
    #     print(des[i][:20].tolist())



