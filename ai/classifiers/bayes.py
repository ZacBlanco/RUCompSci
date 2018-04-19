'''Bayes classifier for data
'''
import math
from data_extractor import read_facedata, read_digitdata
import numpy as np


def generate_model(data):
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
        summaries[field] = (mean, std)
    
    return summaries

def make_prediction(data, model):
    # [print("{} {}".format(k, model[k])) for k in model]
    def gaus_prob(x, mu, sig):
        if sig == 0:
            return 1.0
        return (1/(math.sqrt(2*math.pi*(sig**2))))*math.exp(-.5*(math.pow((x-mu)/sig, 2)))
    guas = lambda x, mu, sig: 1 if (sig == 0) else (1.0/(math.sqrt(2.0*math.pi*(math.pow(sig, 2)))))*math.exp(-.5*(math.pow((float(x)-mu)/sig, 2)))
    npg = np.vectorize(gaus_prob)
    probs = []
    for key in model:
        bclass = model[key] # mu, sigma tuple
        prob = npg(data, bclass[0], bclass[1])
        prob = -1 * np.log(prob) # do log probabilities
        probs.append((key, np.sum(prob)))
    # print(probs)
    return min(probs, key=lambda x: x[1])


if __name__ == "__main__":

    y = read_facedata('train')
    m = generate_model(y)
    d = read_facedata('test')
    # np.set_printoptions(threshold=np.inf)
    total = len(d)
    cor = 0
    for i in range(total):
        cl, lp = make_prediction(d[i][0], m)
        if (cl == d[i][1]):
            cor += 1
    accuracy = float(cor)/total
    print(accuracy)

    y = read_digitdata('training')
    m = generate_model(y)
    d = read_digitdata('training')
    total = len(d)
    cor = 0
    for i in range(total):
        cl, lp = make_prediction(d[i][0], m)
        # print("({}, {})".format(cl, lp))
        if (cl == d[i][1]):
            cor += 1
    accuracy = float(cor)/total
    print(accuracy)

