'''Deep Learning - Fine Tuning Neural Networks

Rutgers University 
Robotics and Computer Vision (Fall 2017) Final Project

Author: Zachary Blanco
Date: 12/14/17

This python module reads reads in a set of imagenet images and reports the accuracy, precision,
recall, etc on the classification of the network

The next part after will fine tune the original network to classify two classes of images: cats and
dogs. The network will be adjusted to only output two classes.

'''
import sys
import os
import random
from keras.applications.resnet50 import ResNet50 as rn50
from keras.preprocessing import image
from keras.applications.resnet50 import preprocess_input, decode_predictions
from keras.layers import Dense, Flatten
from keras.models import Sequential, Model
from keras.optimizers import SGD
from keras.utils import to_categorical
import numpy as np

def load_image(fileloc):
    img = image.load_img(fileloc, target_size=(224, 224))
    x = image.img_to_array(img)
    x = np.expand_dims(x, axis=0)
    x = preprocess_input(x)
    return x

def load_n_imgs(n, ftype=None):
    l = []
    if ftype != 'cat' and ftype != 'dog':
        raise ValueError("ftype must be either 'cat' or 'dog'")
    fstring = 'Images/catsndogs/train/{}.{}.jpg'.format(ftype, '{}')
    samp = range(12499)
    nums = random.sample(samp, n)
    for i in nums:
        l.append(load_image(fstring.format(i)))
    return l

def load_cats(n):
    return load_n_imgs(n, 'cat')

def load_dogs(n):
    return load_n_imgs(n, 'dog')


def load_data(class_size):
    """cats and dogs dataset.
    # Returns
        Tuple of Numpy arrays: `(x_train, y_train), (x_test, y_test)`.
    """
    dirname = 'Images/catsndogs/train'
    totalnum = 12500

    random.randint(0, totalnum-1)
    num_train_samples = class_size*2

    x_train = np.empty((num_train_samples, 224, 224, 3), dtype='uint8')
    y_train = np.empty((num_train_samples,2), dtype='uint8')

    samp_range = range(0, 12499)
    cat_sample = random.sample(samp_range, class_size)
    dog_sample = random.sample(samp_range, class_size)
    ctr = 0
    for x in cat_sample:
        # Load cats
        fname = dirname + '/cat.{}.jpg'.format(x)
        x_train[ctr, :, :, :] = load_image(fname)
        y_train[ctr] = np.array([0, 1])
        ctr += 1

    for x in dog_sample:
        # Load dogs
        fname = dirname + '/dog.{}.jpg'.format(x)
        x_train[ctr, :, :, :] = load_image(fname)
        y_train[ctr] = np.array([1, 0])
        ctr += 1

    return (x_train, y_train)

def part_a():
    network = rn50(weights='imagenet') # default input size is 224x224
    # explore classes: http://image-net.org/explore
    classes = ['Band_Aid', 'forklift', 'running_shoe', 'teapot']
    OTP = 0
    OTN = 0
    OFP = 0
    OFN = 0
    cf = {}
    for x in classes:
        cf[x] = {0:0, 1:0, 2:0, 3:0}
    for classname in classes:
        TP = 0
        TN = 0
        FP = 0
        FN = 0
        for i in range(1, 6):
            x = load_image('Images/keras/{}/img{}.jpg'.format(classname, i))
            preds = network.predict(x)
            p = decode_predictions(preds, top=2)[0][0]
            # print(p[1] == classname)
            # print(p[1])
            predicted_class = str(p[1])
            if predicted_class == classname:
                OTP += 1
                cf[classname][0] += 1
                for x in classes:
                    if x is not classname:
                        cf[x][1] += 1 # Add true negatives
            elif predicted_class in classes and predicted_class != classname:
                cf[predicted_class][2] += 1 # Add false positive to this class
            else:
                OFN += 1
                cf[classname][3] += 1
    
    # Now Part 2 - Calculate matrices
    # Confusion Matrix - 2x2 Actual +- vs Predicted +-
    # Accuracy (TP + TN) / (TP + TN + FP + FN)
    # F Score - (Precision*Recall)/(Precision + Recall)
    # Precision - TP / (TP + FP)
    # Recall - TP / (TP + FN)

    for classname in cf:
        TP = cf[classname][0]
        TN = cf[classname][1]
        FP = cf[classname][2]
        FN = cf[classname][3]
        acc = float(TP+TN) / (TP+TN+FP+FN)
        prec = float(TP) / (TP + FP)
        rec = float(TP) / (TP + FN)
        print("Classifier Performance for {}:".format(classname))
        print("Accuracy: {}".format(acc))
        print("Precision: {}".format(prec))
        print("Recall: {}".format(rec))

    print("Overall Classifier Performance:")
    # print("\tCondition Positive - Condition Negative")
    # print("Predicted Positive: {} \t {}".format(OTP, OFP))
    # print("Predicted Negative: {} \t {}".format(OFN, OTN))
    acc = float(OTP+OTN) / (OTP+OTN+OFP+OFN)
    prec = float(OTP) / (OTP + OFP)
    rec = float(OTP) / (OTP + OFN)
    f_score =  (prec*rec) / (prec + rec)
    print("Accuracy: {}".format(acc))
    print("Precision: {}".format(prec))
    print("Recall: {}".format(rec))
    print("F Score: {}".format(f_score))

def create_fine_tuned():
    network = rn50(weights='imagenet')
    network.layers.pop()
    network.layers[-1].outbound_nodes = []
    network.outputs = [network.layers[-1].output]
    output = network.get_layer('avg_pool').output
    output = Flatten()(output)
    output = Dense(units=2, activation='softmax')(output) # your newlayer Dense(...)
    network = Model(network.input, output)
    sgd = SGD(lr=1e-2, decay=1e-6, momentum=0.9, nesterov=True)
    network.compile(optimizer=sgd, loss='categorical_crossentropy', metrics=['accuracy'])
    return network

def train_network(network, weight_file='t1weights.h5', class_size=100):
    train_data, train_label = load_data(class_size)
    network.fit(train_data, train_label,
                    shuffle=True,
                    verbose=1)
    network.save_weights(weight_file)
    return network


def load_network(weight_file):
    network = create_fine_tuned()
    network.load_weights(weight_file)
    return network

def pred_test(network, num):
    # Predict cats
    cats = load_cats(num)
    # predicted:| cat | dog |
    #           +-----+-----+
    # actual:cat|     |     |
    #           |-----+-----|
    #        dog|     |     |
    #           |-----+-----+
    conf_mat = [[0, 0],
                [0, 0]]
    
    print
    for x in cats:
        if decode_cd(network.predict(x)[0]) == 'cat':
            conf_mat[0][0] += 1
        else:
            conf_mat[0][1] += 1

    # Predict dogs
    dogs = load_dogs(num)
    for x in dogs:
        if decode_cd(network.predict(x)[0]) == 'dog':
            conf_mat[1][1] += 1
        else:
            conf_mat[1][0] += 1

    conf_str = '''
    predicted | cat | dog |
              +-----+-----+
    actual cat| {}  | {}  |
              |-----+-----|
    actual dog| {}  | {}  |
              |-----+-----+
    '''.format(conf_mat[0][0], conf_mat[0][1], conf_mat[1][0], conf_mat[1][1])

    print(conf_str)

def decode_cd(pred):
    '''Decodes Cat and Dog predictions
    '''
    return 'dog' if pred[0] > pred[1] else 'cat'


help_str = '''
    --help                  Display this help message
    
    --imnet                 Uses ResNet50 to classify 20 images. Reports statistics

    --train {weights}       Trains a modified version ResNet50 to recognize cats and dogs
                            weights after training are saved to {weights} if specified.
                            Otherwise, weights are saved to 't1weights.h5'. The starting
                            weights for the network are the original weights from
                            ResNet50. We train on 200 random images from the Cats and
                            Dogs dataset (100 dogs, 100 cats)

    --test {weights} {num}  Loads weights from the {weights} file, then performs
                            predictions on test data. {num} is the number of random
                            samples to predict for each class of cat and dog

    --loadtrain {weights}   Loads weights from the {weights} file, then trains and
                            refines those weights using more training data, then performs
                            predictions on test data. New weights are automatically saved
                            to 't1weights.h5`
'''

def main():
    '''Classifying images and Fine-tuning with Keras.
    '''
    if len(sys.argv) < 2:
        print(help_str)
        print(sys.argv)
        sys.exit()
    args = sys.argv[1:] # Gets only the passed args

    if '--help' == args[0]:
        print(help_str)
        sys.exit()
    
    if '--imnet' == args[0]:
        part_a()
        sys.exit()
    
    weights = args[1] if len(args) > 1 else 't1weights.h5'
    if not os.path.isfile(weights):
        print("ERROR: Weights file {} does not exist".format(weights))
        sys.exit()
    
    if '--train' == args[0]:
        n = create_fine_tuned()
        train_network(n, weight_file=weights)
    elif '--test' == args[0]:
        n = load_network(weights)
        num = int(args[2]) if len(args) > 2 else 10
        pred_test(n, num)
    elif '--loadtrain' == args[0]:
        num = int(args[2]) if len(args) > 2 else 10
        n = create_fine_tuned()
        n.load_weights(weights)
        n = train_network(n)
        pred_test(n, num)
    else:
        print(help_str)

if __name__ == "__main__":
    main()