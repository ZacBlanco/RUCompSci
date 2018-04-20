import numpy as np
from data_utils import binarize

def train(data, epochs=5, learning_rate=.05):
    class_dict = {}
    data = binarize(data)
    # puts all similarly classed data into a "bucket"
    for item in data:
        # print(item)
        if item[1] not in class_dict:
            class_dict[item[1]] = []
        class_dict[item[1]].append(item[0])
    
    size = data[0][0].shape[0]
    wvecs = np.random.rand(len(class_dict), size) # N classes X 724 weights
    for k in range(epochs):
        for d in data:
            # Calculate output vector for the applicable class
            curmax = predict(d[0], wvecs)
            
            # If the matched class isn't the same, update the weight vectors
            if curmax[0] != d[1]:
                # Modify weight vectors for the respective class with the difference and learning rate
                sub_vec = wvecs[curmax[0]]
                add_vec = wvecs[d[1]]
                scaled_err = learning_rate*d[0] # d[0] is a vector of 0's and 1's
                wvecs[curmax[0]] = np.subtract(sub_vec, scaled_err)
                wvecs[d[1]] = np.add(add_vec, scaled_err)
        # [print(wvecs[x]) for x in wvecs]
        # print("Epoch {} done".format(k))
        
    
    return wvecs




def predict(data, model): # Assume binarized data
    curmax = (-1, -10E200)
    scores = model.dot(data) / model.shape[1]
    ind = np.argmax(scores)
    curmax = (ind, scores[ind])
    return curmax