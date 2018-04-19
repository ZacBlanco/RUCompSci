'''test the perceptron and bayesian classifiers
'''
import time
import bayes
import neural
from data_extractor import read_facedata, read_digitdata, take_sample


def main():
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

    pass

if __name__ == "__main__":
    main()