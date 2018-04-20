'''test the perceptron and bayesian classifiers
'''
import time
import bayes
import neural
from data_utils import read_facedata, read_digitdata, take_sample, binarize


def classify_bayes():
    fracs = [x/10 for x in range(1,11)]
    digitd = (read_digitdata('training'), read_digitdata('test'), read_digitdata('validation'))
    faced = (read_facedata('train'), read_facedata('test'), read_facedata('validation'))
    datasets = [faced, digitd]
    # Bayesian
    for dataset in datasets:
        for f in fracs:
            fs = take_sample(dataset[0], f)
            t1 = time.time()
            model = bayes.train(fs)
            t2 = time.time()
            total = len(dataset[1])
            cor = 0
            for item in dataset[1]:
                cl, logprob = bayes.predict(item[0], model)
                if cl == item[1]:
                    cor += 1
            acc = float(cor)/total
            print("Bayes class accuracy frac {} train_time {} accuracy {}".format(f,t2-t1, acc))

def classify_perceptron():
    fracs = [x/10 for x in range(1,11)]
    digitd = (read_digitdata('training'), read_digitdata('test'), read_digitdata('validation'))
    faced = (read_facedata('train'), read_facedata('test'), read_facedata('validation'))
    datasets = [faced, digitd]
    
    # Perceptron
    EPOCHS = 150
    LR = 0.05
    for dataset in datasets:
        for f in fracs:
            fs = take_sample(dataset[0], f)
            t1 = time.time()
            model = neural.train(fs, epochs=EPOCHS, learning_rate=LR)
            t2 = time.time()
            total = len(dataset[1]) # testing dataset
            cor = 0
            for item in binarize(dataset[1]):
                cl, logprob = neural.predict(item[0], model)
                if cl == item[1]:
                    cor += 1
            acc = float(cor)/total
            print("Perceptron class accuracy frac {} train_time {} accuracy {}".format(f,t2-t1, acc))

def main():
    # classify_bayes()
    classify_perceptron()
    

if __name__ == "__main__":
    main()