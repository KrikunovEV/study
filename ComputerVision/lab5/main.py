import json
import cv2
import numpy as np
import matplotlib.pyplot as plt
import torch
from random import shuffle

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

# elementwise mean by default
LossClassification = torch.nn.CrossEntropyLoss()
LossRegression = torch.nn.SmoothL1Loss()

OptimizerCNN_Class = torch.optim.Adam(list(CNN.parameters()) + list(Classification.parameters()), lr=0.00025)
OptimizerRegr = torch.optim.Adam(Regression.parameters(), lr=0.00025)


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


losses = []

for iteration in range(4000):

    ind = np.random.choice(len(trainData), 32)
    input = trainData[ind]
    label = trainLabel[ind]
    coord = trainCoords[ind]

    output = CNN(input)
    class_output = Classification(output)
    regr_output = Regression(output)

    loss1 = LossClassification(class_output, label)
    loss2 = 0.5 * LossRegression(regr_output, coord)
    losses.append(loss1.item() + loss2.item())
    print("Episode: " + str(iteration) + "; Loss: " + str(losses[-1]))

    OptimizerCNN_Class.zero_grad()
    OptimizerRegr.zero_grad()

    (loss1 + loss2).backward()

    OptimizerCNN_Class.step()
    OptimizerRegr.step()

plt.title("Batch losses on train dataset")
plt.plot(np.arange(len(losses)), losses)
plt.show()

torch.save(CNN.state_dict(), 'CNN.pt')
torch.save(Regression.state_dict(), 'Regr.pt')
torch.save(Classification.state_dict(), 'Class.pt')

iter = 1
matrix = np.zeros((10, 10))
for input, label, coord in zip(testData, testLabel, testCoords):
    output = CNN(input[np.newaxis, :, :, :])
    class_output = Classification(output)
    regr_output = np.array(Regression(output).detach().numpy()[0] * 64, dtype=np.uint8)

    input = input.detach().numpy()[0]

    if iter % 100 == 0:
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