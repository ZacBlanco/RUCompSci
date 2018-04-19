'''Extracts the information from each dataset.

facedata:
    - 70 lines per entry.
    - Every 70th line is blank.


digitdata:
    - 28 lines per entry.
    - Every 28th line is blank.
    - Digits actually only appear on up to 19 lines
        - Find the first line which isn't simply just spaces. The next 19 lines contain the digit.
'''
import numpy as np
import random 


def read_facedata(dtype='train'):
    '''Read in the face data into numpy vectors along with their class label as a tuple.

    Returns a list of type (numpy.array, int) where the array represents the data, and the int is the class label.

    Possible options for dtype are 'train', 'test', and 'validation'
    '''
    facedir = 'facedata/'
    fn = 'facedata'
    data_file = facedir + fn + dtype
    label_file = data_file + 'labels'

    raw_features = []
    labeled_data = []

    with open(data_file, 'r') as f:
        data = f.readlines()
        num_lines = len(data)
        num_items = int(num_lines/70)
        for x in range(num_items):
            ind = x*70
            feature = data[ind:(ind+70)] # Get all of the lines pertaining for 
            feature_string = list(''.join(feature).replace('\n', ''))
            # print('item {}'.format(x))
            # print(feature_string)
            feat = list(map(lambda x: ord(x), feature_string)) # Convert into a list of numbers
            np_feat = np.array(feat) # convert into a numpy array
            raw_features.append(np_feat) # append numpy array to object vector


    num_items = len(raw_features) # Total number of labeled items
    with open(label_file, 'r') as f:
        # Now label each "raw feature" and put it in labeled_data

        for x in range(num_items):
            label = int(f.readline())
            item =(raw_features[x], label)
            labeled_data.append(item)
    
    return labeled_data


def read_digitdata(dtype='training'):
    '''Read in the digit data into numpy vectors along with their class label as a tuple.

    Returns a list of type (numpy.array, int) where the array represents the data, and the int is the class label.

    Possible options for dtype are 'training', 'test', and 'validation'
    '''
    digit_dir ='digitdata/'
    data_file = digit_dir + dtype + 'images'
    label_file = digit_dir + dtype + 'labels'

    raw_features = []
    labeled_data = []

    with open(data_file, 'r') as f:
        data = f.readlines()
        num_lines = len(data)
        num_items = int(num_lines/28)
        for x in range(num_items):
            ind = x*28
            feature = data[ind:(ind+28)] # Get all of the lines pertaining for

            # Tested refinement --- it didn't improve bayesian classification.
            # Refine to get the 19 lines where the first line includes the digit.
            # for i in range(9):
            #     if len(feature[i].replace('\n', '').replace(' ', '')) != 0: # line isn't a space
            #         break

            # refined_feature = feature[i:i+19]
            refined_feature = feature

            feature_string = list(''.join(refined_feature).replace('\n', ''))
            # print('item {}'.format(x))
            # print(feature_string)
            feat = list(map(lambda x: ord(x), feature_string)) # Convert into a list of numbers
            np_feat = np.array(feat) # convert into a numpy array
            raw_features.append(np_feat) # append numpy array to object vector


    num_items = len(raw_features) # Total number of labeled items
    with open(label_file, 'r') as f:
        # Now label each "raw feature" and put it in labeled_data

        for x in range(num_items):
            label = int(f.readline())
            item =(raw_features[x], label)
            labeled_data.append(item)
    return labeled_data


def take_sample(dataset, frac):
    '''Take a sample of data from the dataset
    '''
    newset = []
    for item in dataset:
        v = random.random()
        if v < frac:
            newset.append(item)
    return newset
    

# Test reading of data
if __name__ == "__main__":
    # x = read_facedata('validation')
    # [print(z) for z in x]
    # print(len(x))

    y = read_digitdata('validation')
    # [print(z) for z in y]
    print(len(y))


