import cv2
import numpy as np

def disp_key(data, key, width=None, height=None):
	cv2.imshow('img', data)
	if width != None and height != None:
		cv2.resizeWindow('img', int(width), int(height))
	while True:
		k = cv2.waitKey(50) & 0xFF
		if k == ord(key):
			break
	cv2.destroyAllWindows()

def problem1():
    IMAGE = 'hat.jpg'
    a = .02
    b = .01
    c = 10
    d = .01
    e = -.02
    f = 5
    affine_mat = np.matrix([[a, b, c],
                            [d, e, f],
                            [0, 0, 1]])
    img = cv2.imread(IMAGE, cv2.IMREAD_GRAYSCALE)
    while img.shape[0] > 500 and img.shape[1] > 500:
        img = cv2.pyrDown(img)
    # disp_key(img, 'n')
    # print(img.shape)
    (rows, cols) = img.shape[:2]
    mapx = np.zeros((rows, cols), np.float32)
    mapy = np.zeros((rows, cols), np.float32)
    print(affine_mat)        
    for r in range(rows):
        for c in range(cols):
            pt = np.array([c, r, 1])
            uv = affine_mat.dot(pt).T
            mapx[r, c] = c + uv[0]
            mapy[r, c] = r + uv[1]

    newimg = cv2.remap(img, mapx, mapy, 1)
    disp_key(img, 'n')
    disp_key(newimg, 'n')
    disp_key(newimg-img, 'n')
    return (img, newimg)

def problem2(img1, img2, size, pyrlevels):
    params = np.array([0, 0, 0, 0, 0, 0])
    i1 = img1
    i2 = img2
    for i in range(pyrlevels):
        params = lk_method(i1, i2, size, params)

def lk_method(img1, img2, size, params0):
    rows, cols = size
    ix = np.zeros(size, np.float32)
    iy = np.zeros(size, np.float32)
    ts = np.zeros(size, np.float32)
    for x in range(0, cols):
        for y in range(0, rows):
            # get ix, iy, and it for every pixel in the image.
            # Store in ix, iy, ts
            ct = 0
            S = np.zeros((9, 2), np.float32)
            T = np.zeros((9, 1), np.float32)
            for dx in range(-1, 2):
                for dy in range(-1, 2):
                    ixv = 0
                    iyv = 0
                    itv = 0
                    try:
                        ixv = (float(img2[y+dy, x+dx+1]) - float(img1[y+dy, x+dx])) / 2.0
                    except: pass
                    try:
                        iyv = (float(img2[y+dy+1, x+dx]) - float(img1[y+dy, x+dx])) / 2.0
                    except: pass
                    try:
                        itv = float(img2[y+dy, x+dx]) - float(img1[y+dy, x+dx])
                    except: pass
                    ct += 1
                    S[ct-1, 0] = ixv
                    S[ct-1, 1] = iyv
                    T[ct-1] = -1*itv
            try:
                uvals = np.linalg.inv(S.T.dot(S)).dot(S.T).dot(T)
            except:
                uvals = np.linalg.pinv(S.T.dot(S)).dot(S.T).dot(T)
            ix[y, x] = uvals[0]
            iy[y, x] = uvals[1]
            ts[y, x] = -itv
	
    print("Finished ix, iy calculations")
    aTa = np.zeros((6, 6))
    aTb = np.zeros((6, 1))
    for c in range(cols):
        for r in range(rows):
            it = ts[r, c]
            ixx = ix[r, c]
            iyy = iy[r, c]
            vec = np.matrix([ixx*c, ixx*r, ixx, iyy*c, iyy*r, iyy], np.float32).T
            prod = vec.dot(vec.reshape(1, 6))
            
            v2 = np.matrix([it*ixx*c, it*ixx*r, it*ixx, it*iyy*c, it*iyy*r, it*iyy], np.float32)

            aTa += prod
            aTb -= v2.reshape(6, 1)
    
    params = aTa.dot(aTb)
    return params
   

def main():
    im1, im2 = problem1()
    problem2(im1, im2, (im1.shape[0],200), 2)
    

if __name__ == "__main__":
    main()