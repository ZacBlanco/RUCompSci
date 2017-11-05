import numpy as np
import cv2

def disp_key(data, key, width=None, height=None):
    cv2.imshow('img', data)
    if width != None and height != None:
        cv2.resizeWindow('img', int(width), int(height))
    while True:
        k = cv2.waitKey(50) & 0xFF
        if k == ord(key):
            break

def normalize_filt(filt):
    s = reduce(lambda x, y: x+y, filt)
    norm = map(lambda x: float(x)/s, filt)
    return norm

def downsample(img):
    return cv2.pyrDown(img)

def pyr(layers, filt, img):
    imgs = []
    blurs = []
    dog = cv2.imread(img, cv2.IMREAD_GRAYSCALE)
    filt = np.array(normalize_filt(filt))
    imgs.append(dog)
    for i in range(layers):
        blur1 = cv2.sepFilter2D(imgs[i], -1, filt.T, filt)
        blurs.append(blur1)
        ds1 = downsample(blur1)
        imgs.append(ds1)
    
    return imgs, blurs

def problem1(filename):
    imgs, blurs = pyr(4, [1, 4, 6, 4, 1], filename)
    disp_key(imgs[0], 'n')
    for i in range(len(blurs)):
        disp_key(blurs[i], 'n')
        disp_key(imgs[i+1], 'n')

def problem2(filename):
    imgs, blurs = pyr(4, [1, 4, 6, 4, 1], filename)
    for i in range(len(blurs)):
        # print(imgs[i])
        # print(blurs[i])
        # print(imgs[i] - blurs[i])
        disp_key((imgs[i] - blurs[i])/float(255), 'n')

def problem3():
    from keras.datasets import mnist
    (train_images, train_labels), (test_images, test_labels) = mnist.load_data()
    print(train_images.shape)
    print(len(train_labels))
    print(train_labels)
    print(test_images.shape)
    print(len(test_labels))
    print(test_labels)
    
    from keras import models
    from keras import layers
    network = models.Sequential()
    network.add(layers.Dense(512, activation='relu', input_shape=(28 * 28,)))
    network.add(layers.Dense(10, activation='softmax'))
    network.compile(optimizer='rmsprop',
    loss='categorical_crossentropy',
    metrics=['accuracy'])
    train_images = train_images.reshape((60000, 28 * 28))
    train_images = train_images.astype('float32') / 255
    test_images = test_images.reshape((10000, 28 * 28))
    test_images = test_images.astype('float32') / 255
    from keras.utils import to_categorical
    train_labels = to_categorical(train_labels)
    test_labels = to_categorical(test_labels)
    
    network.fit(train_images, train_labels, epochs=5, batch_size=128)
    
    test_loss, test_acc = network.evaluate(test_images, test_labels)
    print('\n')
    print('test_acc:', test_acc)

def main():
    FILENAME = 'dog.jpg'
    problem1(FILENAME)
    problem2(FILENAME)
    problem3()

if __name__ == "__main__":
    main()