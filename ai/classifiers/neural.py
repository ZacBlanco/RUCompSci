import numpy as np

def train(data, epochs=5, learning_rate=.1):
    class_dict = {}

    # puts all similarly classed data into a "bucket"
    for item in data:
        # print(item)
        if item[1] not in class_dict:
            class_dict[item[1]] = []
        class_dict[item[1]].append(item[0])
    
    size = data[0][0].shape[0]
    wvecs = {}
    for dclass in class_dict:
        wvecs[dclass] = np.random.rand(size,1)

    for k in range(epochs):
        for d in data:
            # Calculate output vector for the applicable class
            curmax = predict(d[0], wvecs)
            # curmax = (-10E200, -1)
            # for cl in class_dict:
            #     wv = wvecs[cl]
            #     scores = np.dot(d[0], wv)
            #     tot = np.sum(scores)
            #     if tot > curmax[0]:
            #         curmax = (tot, cl)
            
            # If the mathed class isn't the same, update the weight vectors
            if curmax[0] != d[1]:
                # Modify weight vectors for the respective class with the difference and learning rate
                sub_vec = wvecs[curmax[0]]
                add_vec = wvecs[d[1]]
                vec = learning_rate*d[0]
                wvecs[curmax[0]] = np.subtract(sub_vec, vec)
                wvecs[d[1]] = np.add(add_vec, vec)
        print("Epoch {} done".format(k))
        
    
    return wvecs




def predict(data, model):
    curmax = (-1, -10E200)
    for cl in model:
        wv = model[cl]
        scores = np.dot(data, wv)
        tot = np.sum(scores)
        if tot > curmax[1]:
            curmax = (cl, tot)
    return curmax