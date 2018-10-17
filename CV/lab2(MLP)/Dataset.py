import numpy as np
from sklearn.datasets import load_digits
import pickle


def pickle_it(data, path):
    with open(path, 'wb') as f:
        pickle.dump(data, f, protocol=pickle.HIGHEST_PROTOCOL)

def unpickle_it(path):
    with open(path, 'rb') as f:
        return pickle.load(f)



def GetDataForClassification():
    dig = load_digits()
    images = dig['images']
    target = dig['target']

    # DATA ALREADY SHUFFLED INTO DATASET

    # normalize
    half = np.max(images) / 2
    images = (images - half) / half

    trainData, trainLabel = [], []
    validData, validLabel = [], []
    testData, testLabel = [], []

    one_hot_encoding = np.zeros(10)

    for k in range(10):
        mask = target == k
        img_k = images[mask]
        label_k = target[mask]

        num_train = int(len(label_k) * 0.8)
        num_valid = num_train + int(len(label_k) * 0.1)
        num_test = int(len(label_k))

        label = np.array(one_hot_encoding)
        label[k] = 1.0

        for i in range(num_train):
            trainData.append(img_k[i].flatten())
            trainLabel.append(label)

        for i in range(num_train, num_valid):
            validData.append(img_k[i].flatten())
            validLabel.append(label)

        for i in range(num_valid, num_test):
            testData.append(img_k[i].flatten())
            testLabel.append(label)

    return np.array(trainData), np.array(trainLabel), np.array(validData), np.array(validLabel), np.array(testData), np.array(testLabel)


def GetDataForRegression(DataLen):

    # by law:
    # Y = np.sin(2 * pi * X) + np.exp(x)

    TrainDataLen = int(DataLen * 0.9)

    trainData = np.empty((TrainDataLen, 2))
    trainData[:, 0] = np.linspace(0, 1, TrainDataLen)
    trainData[:, 1] = np.sin(2* np.pi * trainData[:, 0]) + np.exp(trainData[:, 0]) + np.random.randn(TrainDataLen) * 0.1

    testData = np.empty((DataLen - TrainDataLen, 2))
    testData[:, 0] = np.linspace(0, 1, DataLen - TrainDataLen)
    testData[:, 1] = np.sin(2 * np.pi * testData[:, 0]) + np.exp(testData[:, 0]) + np.random.randn(DataLen - TrainDataLen) * 0.1

    return trainData, trainData[:, 1], testData, testData[:, 1]
