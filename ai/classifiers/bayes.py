'''Bayes classifier for data
'''
import math
from data_utils import read_facedata, read_digitdata
import numpy as np


def train(data):
    '''Given a dataset, generate the bayesian model which can be used to make inferential predictions on the class of a dataset
    
    Arguments:
        data: A list of tuples which are (value, class) pairs
    '''

    class_dict = {}

    # puts all similarly classed data into a "bucket"
    for item in data:
        # print(item)
        if item[1] not in class_dict:
            class_dict[item[1]] = []
        class_dict[item[1]].append(item[0])

    # Once in the bucket we need to summarize our data
    summaries = {}
    for field in class_dict:
        dat = class_dict[field]
        mat = np.column_stack(dat).T # transpose for column stacking
        mean = np.mean(mat, axis=0)
        std = np.std(mat, axis=0)
        summaries[field] = (mean, std, len(dat))
    
    return summaries

def predict(data, model):
    def gaus_prob(x, mu, sig):
        k = .1
        rv = k
        if sig != 0:
            val = (1.0/(math.sqrt(2.0*math.pi*(sig**2))))*math.exp(-.5*(math.pow((float(x)-mu)/sig, 2)))
            if val == 0:
                rv = 1E-300 # If we get a probability of 0, just set it to something tiny so when we take the log it is larger
            else:
                rv = val
        return rv
    npg = np.vectorize(gaus_prob)
    probs = []
    for key in model:
        bclass = model[key] # mu, sigma tuple
        prob = npg(data, bclass[0], bclass[1])
        prob = -np.log(prob) # do log probabilities
        prob = np.sum(prob)# - bclass[2]
        probs.append((key, prob))
    # print(probs)
    return min(probs, key=lambda x: x[1])


if __name__ == "__main__":

    y = read_facedata('train')
    m = train(y)
    d = read_facedata('test')
    # np.set_printoptions(threshold=np.inf)
    total = len(d)
    cor = 0
    for i in range(total):
        cl, lp = predict(d[i][0], m)
        if (cl == d[i][1]):
            cor += 1
    accuracy = float(cor)/total
    print(accuracy)

    y = read_digitdata('training')
    m = train(y)
    d = read_digitdata('test')
    cor = 0
    for i in range(total):
        # print(i)
        cl, lp = predict(d[i][0], m)
        # print("({}, {})".format(cl, lp))
        if (cl == d[i][1]):
            cor += 1
    accuracy = float(cor)/total
    print(accuracy)

