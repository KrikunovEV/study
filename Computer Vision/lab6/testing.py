import json
import torch
import numpy as np
import matplotlib.pyplot as plt
from random import shuffle
import cv2

def IoU(c1, c2):
    Ix0 = max(c1[0], c2[0])
    Iy0 = max(c1[1], c2[1])
    Ix1 = min(c1[2], c2[2])
    Iy1 = min(c1[3], c2[3])

    if Ix0 > Ix1 or Iy1 < Iy0:
        return 0

    S1 = (c1[2] - c1[0]) * (c1[3] - c1[1])
    S2 = (c2[2] - c2[0]) * (c2[3] - c2[1])

    SI = (Ix1 - Ix0) * (Iy1 - Iy0)
    SU = S1 + S2 - SI

    return SI / SU

# NN module to flatten partial data
class Flatten(torch.nn.Module):
    def forward(self, input):
        return input.view(input.size(0), -1)

CNN = torch.nn.Sequential(
    torch.nn.Conv2d(1, 8, 3, padding=1),
    torch.nn.ReLU(),
    torch.nn.Conv2d(8, 8, 3, padding=1),
    torch.nn.ReLU(),
    torch.nn.MaxPool2d(2),
    torch.nn.Conv2d(8, 16, 3, padding=1),
    torch.nn.ReLU(),
    torch.nn.Conv2d(16, 16, 3, padding=1),
    torch.nn.ReLU(),
    torch.nn.Conv2d(16, 16, 3, padding=1),
    torch.nn.ReLU(),
    torch.nn.MaxPool2d(2),
    torch.nn.Conv2d(16, 32, 3, padding=1),
    torch.nn.ReLU(),
    torch.nn.Conv2d(32, 32, 3, padding=1),
    torch.nn.ReLU(),
    torch.nn.Conv2d(32, 32, 3, padding=1),
    torch.nn.ReLU(),
    torch.nn.MaxPool2d(2),
    Flatten(),
    torch.nn.Linear(8 * 8 * 32, 512),
    torch.nn.ReLU()
)

Classification = torch.nn.Sequential(
    torch.nn.Linear(512, 10),
    torch.nn.Softmax(dim=-1)
)

Regression = torch.nn.Sequential(
    torch.nn.Linear(512, 4),
)

CNN.load_state_dict(torch.load("CNN.pt"))
Classification.load_state_dict(torch.load("Class.pt"))
Regression.load_state_dict(torch.load("Regr.pt"))


# Download dataset
path = "C:/Users/Evgeniy/Desktop/study/10k/"
f1 = open(path + "images.json", 'r')
f2 = open(path + "labels.json", 'r')
f3 = open(path + "coords.json", 'r')

# size, num channels(1), width, height
images = np.array(json.load(f1))[:, np.newaxis, :, :]
labels = np.array(json.load(f2))
coords = (np.array(json.load(f3)).reshape((-1, 4)))

f1.close()
f2.close()
f3.close()


# shuffle
ind = np.arange(10000)
shuffle(ind)
images = images[ind]
labels = labels[ind]
coords = coords[ind]

# Divide dataset on train(85)/test(15)
trainData, trainLabel, trainCoords = images[:8500], labels[:8500], coords[:8500]
testData, testLabel, testCoords = images[8500:], labels[8500:], coords[8500:]

trainData, trainLabel, trainCoords = torch.Tensor(trainData), torch.LongTensor(trainLabel), torch.Tensor(trainCoords)
testData, testLabel, testCoords = torch.Tensor(testData), torch.LongTensor(testLabel), torch.Tensor(testCoords)



iter = 1
F = 0
TP_sample = [[], [] , [] , [], [], [], [], [], [], []]
for input, label, coord in zip(testData, testLabel, testCoords):
    output = CNN(input[np.newaxis, :, :, :])
    class_output = Classification(output).detach().numpy()[0]
    regr_output = np.array(Regression(output).detach().numpy()[0] * 64, dtype=np.uint)

    if IoU(coord.detach().numpy(), regr_output) > 0.5:
        TP_sample[label].append(class_output)
    else:
        F += 1

    input = input.detach().numpy()[0]

    if iter % 100000 == 0:
        input = cv2.rectangle(input, (regr_output[0], regr_output[1]),
                              (regr_output[2], regr_output[3]), color=(255, 0, 0), thickness=1)
        input = cv2.rectangle(input, (coord[0], coord[1]),
                              (coord[2], coord[3]), color=(255, 0, 0), thickness=1)
        plt.imshow(input, cmap='gray')
        plt.show()
    iter += 1

print(1500 - F, F)

AP = []
for k, Sample in enumerate(TP_sample):
    Recall, Precision = [], []

    for p in np.linspace(0, 1, 10):

        TP, FN, FP = 0, 0, 0
        for vector in Sample:
            predicted = np.argmax(vector)

            if predicted == k:
                if vector[predicted] > p:
                    TP += 1
                else:
                    FP += 1
            else:
                if vector[predicted] <= p:
                    FN += 1

        FN += F
        FP += F

        if TP == 0 and FP == 0:
            Precision.append(1)
        else:
            Precision.append(TP / (TP + FP))

        if TP == 0 and FN == 0:
            Recall.append(1)
        else:
            Recall.append(TP / (TP + FN))

    ar = sorted(zip(Recall, Precision))
    Recall = [x for x,y in ar]
    Precision = [y for x, y in ar]

    # smooth, increase accuracy :)
    for i in reversed(range(len(Precision) - 1)):
        if Precision[i] < Precision[i+1]:
            Precision[i] = Precision[i+1]

    S = 0
    for i in range(1, len(Precision)):
        S += Precision[i] * (Recall[i] - Recall[i-1])

    AP.append(S)

print("MAP: ", np.sum(AP) / len(AP))

