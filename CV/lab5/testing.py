import json
import torch
import numpy as np
import matplotlib.pyplot as plt
from random import shuffle
import cv2

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
coords = (np.array(json.load(f3)).reshape((-1, 4))) / 64.0

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
matrix = np.zeros((10, 10))
for input, label, coord in zip(testData, testLabel, testCoords):
    output = CNN(input[np.newaxis, :, :, :])
    class_output = Classification(output)
    regr_output = np.array(Regression(output).detach().numpy()[0] * 64, dtype=np.uint8)

    input = input.detach().numpy()[0]

    if iter % 100 == 0:
        #input = cv2.rectangle(input, (coord[0] * 64, coord[1] * 64), (coord[2] * 64, coord[3] * 64),
                              #color=(255, 0, 0), thickness=1)
        input = cv2.rectangle(input, (regr_output[0], regr_output[1]),
                              (regr_output[2], regr_output[3]), color=(255, 0, 0), thickness=1)
        plt.imshow(input, cmap='gray')
        plt.show()
    iter += 1

    matrix[label, torch.argmax(class_output)] += 1

Accuracy = 0
for i in range(10):
    Accuracy += matrix[i, i]
Accuracy /= np.sum(matrix)

print("Accuracy:", Accuracy)
print("Confusion matrix: ")
print(matrix)