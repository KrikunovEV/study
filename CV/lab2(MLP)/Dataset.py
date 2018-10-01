import numpy as np


def GetDataForClassification(DataLen):

    #spheres

    # 3 classes:
    # the first - into sphere with radius 0.5
    # the second - onto donut between 0.5 - 1.0 radiuses
    # the third - out sphere with radius 1.0

    TrainDataLen = int(DataLen * 0.9)

    Data = np.random.random_sample((DataLen, 3)) * 2 - 1 # NORMALIZE COORDINATE [-1; 1] + ZERO CENTERED
    Label = np.empty(shape=(DataLen, 3))
    for i in range(DataLen):
        norm = np.sqrt(np.sum(np.square(Data[i])))
        if norm < 0.5:
            Label[i] = np.array([1, 0, 0])
        elif norm < 1.0:
            Label[i] = np.array([0, 1, 0])
        else:
            Label[i] = np.array([0, 0, 1])

    return Data[:TrainDataLen], Label[:TrainDataLen], Data[TrainDataLen:], Label[TrainDataLen:]


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
