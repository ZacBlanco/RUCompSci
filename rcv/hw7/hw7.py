import cv2
import numpy as np
import keras
from math import floor

def disp_key(data, key, width=None, height=None):
	cv2.imshow('img', data)
	if width != None and height != None:
		cv2.resizeWindow('img', int(width), int(height))
	while True:
		k = cv2.waitKey(50) & 0xFF
		if k == ord(key):
			break
	cv2.destroyAllWindows()

def normalize_filt(filt):
    s = reduce(lambda x, y: x+y, filt)
    norm = map(lambda x: float(x)/s, filt)
    return norm

def grad(img, img2, y, x):
    ct = 0
    S = np.zeros((9, 2), np.float32)
    T = np.zeros((9, 1), np.float32)
    for dx in range(-1, 2):
        for dy in range(-1, 2):
            ixv = 0
            iyv = 0
            try:
                ixv = (float(img[y+dy, x+dx+1]) - float(img[y+dy, x+dx])) / 2.0
            except: pass
            try:
                iyv = (float(img[y+dy+1, x+dx]) - float(img[y+dy, x+dx])) / 2.0
            except: pass
            try:
                itv = float(img2[y+dy, x+dx]) - float(img[y+dy, x+dx])
            except: pass
            ct += 1
            S[ct-1, 0] = ixv
            S[ct-1, 1] = iyv
            T[ct-1] = -1*itv

    try:
        uvals = np.linalg.inv(S.T.dot(S)).dot(S.T).dot(T)
    except:
        # uvals = np.linalg.pinv(S.T.dot(S)).dot(S.T).dot(T)
        uvals = np.array([0, 0], np.float32).reshape((2, 1))
    ix = uvals[0]
    iy = uvals[1]

    return (ix, iy)

def getsubpix(img, x, y):
    '''Gets the value at "real" location using bilinear interpolation'''
    sy, sx = img.shape[:2]
    sx -= 1
    sy -= 1
    x = max([x, 0])
    y = max([y, 0])
    x = min([x, sx])
    y = min([y, sy])
    x0 = int(max([floor(x), 0]))
    y0 = int(max([floor(y), 0]))
    x1 = int(min([x0+1, sx]))
    y1 = int(min([y0+1, sy]))
    
    v1 = img[y0, x0]
    v2 = img[y0, x1]
    v3 = img[y1, x0]
    v4 = img[y1, x1]
    
    x0 = floor(x)
    x1 = x0 + 1
    y0 = floor(y)
    y1 = y0 + 1
    
    va = (x-x0)*v2 + (x1-x)*v1
    vb = (x-x0)*v4 + (x1-x)*v3

    return (y-y0)*v2 + (y1-y)*v1

def problem1(imgname):
    IMAGE = imgname
    a = .02
    b = .01
    c = 10
    d = .01
    e = -.02
    f = 5
    affine_mat = np.matrix([[a+1, b, c],
                            [d, e+1, f]]) # +1 for affine warp function
                            # [0, 0, 1]])
    img = cv2.imread(IMAGE, cv2.IMREAD_GRAYSCALE)
    while img.shape[0] > 750 and img.shape[1] > 750: # Ensures that the image isn't too large
        img = cv2.pyrDown(img)
    (rows, cols) = img.shape[:2]
    newimg = cv2.warpAffine(img, affine_mat, (cols, rows))
    disp_key(img, 'n')
    disp_key(newimg, 'n')
    disp_key(newimg-img, 'n')
    return (img, newimg)


def problem2(img1, img2, size, pyrlevels, niters):
    # params = np.random.rand(6, 1)
    params =  np.zeros((6, 1))
    params.shape = (6, 1)
    i1 = img1
    i2 = img2
    for i in range(pyrlevels):
        i1 = img1
        i2 = img2
        for i in range(pyrlevels - i):
            i1 = cv2.pyrDown(i1) # pyrDown already blurs the image for us
            i2 = cv2.pyrDown(i2)
        size = i1.shape[:2]
        # disp_key(i1, 'n')
        # disp_key(i2, 'n')
        params = lk_method(i1, i2, size, params, niters)
        # params[2] = params[2] * 2
        # params[5] = params[5] * 2
        # i1 = cv2.sepFilter2D(i1, -1, filt.T, filt)
        # i2 = cv2.sepFilter2D(i2, -1, filt.T, filt)
        
        print("Params after level {}".format(i))
        print(params)
    
    print("FINAL PARAMETERS")
    print(params)

def lk_method(img1, img2, size, params0, niters):
    rows, cols = size
    arrowimg = np.copy(img1) #np.zeros((size[0], size[1], 3))
    aTa = np.zeros((6, 6))
    Agrad = np.zeros((rows, cols, 2)) # Stores ix and iy for every pixel
    params = params0
    pinvA = False
    for c in range(cols):
            for r in range(rows):
                ix, iy = grad(img1, img2, r, c)
                ixx = ix
                iyy = iy
                Agrad[r, c, 0] = ix
                Agrad[r, c, 1] = iy
                vec = np.matrix([ixx*c, ixx*r, ixx, iyy*c, iyy*r, iyy], np.float32)
                prod = vec.dot(vec.reshape(1, 6))
                aTa += prod
    try:
        aTa = np.linalg.inv(aTa)
    except:
        print("WARNING: Pseudo-Inversed A in LK method")
        pinVA = True
        aTa = np.linalg.pinv(aTa)

    for n in range(niters):
        aTb = np.ones((6, 1), np.float32)*.01
        for c in range(cols):
            for r in range(rows):
                uu = params[0]*c + params[1]*r + params[2]
                vv = params[3]*c + params[4]*r + params[5]
                it = float(img2[r, c]) - float(getsubpix(img1, c+uu, r+vv)) # uses bilinear interpolation
                ixx = Agrad[r, c, 0].item()
                iyy = Agrad[r, c, 1].item()
                v2 = np.matrix([it*ixx*c, it*ixx*r, it*ixx, it*iyy*c, it*iyy*r, it*iyy], np.float32)
                aTb -= v2.reshape(6, 1)
                # if (r % 10 == 0 and c % 10 == 0 and n == niters-1):
                    # cv2.arrowedLine(arrowimg, (c, r), (c+uu, r+vv), (255, 255, 255), 1)
    
        params += aTa.dot(aTb)
        # if n == niters -1:
        #     disp_key(arrowimg, 'n')
    pa = params.reshape(2, 3)
    rows, cols = img1.shape
    disp_key(cv2.warpAffine(img1, pa, (cols, rows)), 'n')
    return params
   
def problem3():
    # https://keras.io/applications/#resnet50
    from keras.applications.resnet50 import ResNet50 as rn50
    from keras.preprocessing import image
    from keras.applications.resnet50 import preprocess_input, decode_predictions
    network = rn50(weights='imagenet') # default input size is 224x224
    # explore classes: http://image-net.org/explore
    classes = ['alp', 'acoustic_guitar', 'hare', 'stingray']
    TP = 0
    TN = 0
    FP = 0
    FN = 0
    for classname in classes:
        for i in range(1, 6):
            img = image.load_img('images/{}/img{}.jpg'.format(classname, i), target_size=(224, 224))
            x = image.img_to_array(img)
            x = np.expand_dims(x, axis=0)
            x = preprocess_input(x)
            preds = network.predict(x)
            p = decode_predictions(preds, top=1)[0][0]
            # print(p[1] == classname)
            # print(p[1])
            predicted_class = p[1]
            if str(predicted_class) == classname:
                TP += 1
            else:
                FN += 1

    # Now Part 2 - Calculate matrices
    # Confusion Matrix - 2x2 Actual +- vs Predicted +-
    # Accuracy (TP +TN) / (TP+TN+FP+FN)
    # F Score - (Precision*Recall)/(Precision + Recall)
    # Precision - TP / (TP + FP)
    # Recall - TP / (TP + FN)

    print("Confusion Matrix:")
    print("\tCondition Positive - Condition Negative")
    print("Predicted Positive: {} \t {}".format(TP, FP))
    print("Predicted Negative: {} \t {}".format(FN, TN))
    acc = float(TP+TN) / (TP+TN+FP+FN)
    prec = float(TP) / (TP + FP)
    rec = float(TP) / (TP + FN)
    f_score =  (prec*rec) / (prec + rec)
    print("Accuracy: {}".format(acc))
    print("Precision: {}".format(prec))
    print("Recall: {}".format(rec))
    print("F Score: {}".format(f_score))
        
        

        




def main():
    IMAGE = 'hat.jpg'
    im1, im2 = problem1(IMAGE)
    problem2(im1, im2, im1.shape[:2], 4, 10)
    problem3()

    # ix = np.zeros(size, np.float32)
    # iy = np.zeros(size, np.float32)
    # ts = np.zeros(size, np.float32)
    # for x in range(0, cols):
    #     for y in range(0, rows):
    #         # get ix, iy, and it for every pixel in the image.
    #         # Store in ix, iy, ts
    #         ct = 0
    #         S = np.zeros((9, 2), np.float32)
    #         T = np.zeros((9, 1), np.float32)
    #         for dx in range(-1, 2):
    #             for dy in range(-1, 2):
    #                 ixv = 0
    #                 iyv = 0
    #                 itv = 0
    #                 try:
    #                     ixv = (float(img2[y+dy, x+dx+1]) - float(img1[y+dy, x+dx])) / 2.0
    #                 except: pass
    #                 try:
    #                     iyv = (float(img2[y+dy+1, x+dx]) - float(img1[y+dy, x+dx])) / 2.0
    #                 except: pass
    #                 try:
    #                     itv = float(img2[y+dy, x+dx]) - float(img1[y+dy, x+dx])
    #                 except: pass
    #                 ct += 1
    #                 S[ct-1, 0] = ixv
    #                 S[ct-1, 1] = iyv
    #                 T[ct-1] = -1*itv
    #         try:
    #             uvals = np.linalg.inv(S.T.dot(S)).dot(S.T).dot(T)
    #         except:
    #             uvals = np.linalg.pinv(S.T.dot(S)).dot(S.T).dot(T)
    #         ix[y, x] = uvals[0]
    #         iy[y, x] = uvals[1]
    #         ts[y, x] = -itv  
    # CODE BY ZACHARY BLANCO - 11/28/2017  

if __name__ == "__main__":
    main()


