import numpy as np
import matplotlib.pyplot as plt
import torch
from sklearn import datasets

CNN = torch.nn.Sequential(
    torch.nn.Conv2d(1, 8, 3, padding=1),
    torch.nn.Conv2d(8, 8, 3, padding=1),
    torch.nn.MaxPool2d(2),
    torch.nn.Conv2d(8, 16, 3, padding=1),
    torch.nn.Conv2d(16, 16, 3, padding=1),
    torch.nn.Conv2d(16, 16, 3, padding=1),
    torch.nn.MaxPool2d(2),
)

FC = torch.nn.Sequential(
    torch.nn.Linear(2 * 2 * 16, 32),
    torch.nn.Linear(32, 10),
    torch.nn.Softmax(dim=-1)
)

# elementwise mean by default
Loss = torch.nn.CrossEntropyLoss()

Optimizer = torch.optim.Adam(list(CNN.parameters()) + list(FC.parameters()), lr=0.002)


# Download dataset
digits = datasets.load_digits()
images = digits.images
labels = digits.target


# PREPROCESS
# size, num channels, width, height
images = images[:, np.newaxis, :, :]


# Divide dataset on train(85)/test(15)
trainData, trainLabel = [], []
testData, testLabel = [], []

for k in range(10):
    mask = labels == k
    img_k = images[mask]
    label_k = labels[mask]

    num_train = int(len(label_k) * 0.85)
    num_test = len(label_k)

    for i in range(num_train):
        trainData.append(img_k[i])
        trainLabel.append(label_k[i])

    for i in range(num_train, num_test):
        testData.append(img_k[i])
        testLabel.append(label_k[i])



trainData = torch.Tensor(trainData)
testData = torch.Tensor(testData)
trainLabel = torch.LongTensor(trainLabel)
testLabel = torch.LongTensor(testLabel)


losses = []

for iteration in range(500):

    ind = np.random.choice(len(trainData), 128)
    input = trainData[ind]
    label = trainLabel[ind]

    output = CNN(input)
    output = output.view(len(output), -1) # flatten
    output = FC(output)

    loss = Loss(output, label)
    losses.append(loss.item())

    Optimizer.zero_grad()
    loss.backward()
    Optimizer.step()

plt.title("Batch losses on train dataset")
plt.plot(np.arange(len(losses)), losses)
plt.show()


matrix = np.zeros((10, 10))
for input, label in zip(testData, testLabel):
    output = CNN(input[np.newaxis, :, :, :])
    output = output.view(len(output), -1)  # flatten
    output = FC(output)

    matrix[label, torch.argmax(output)] += 1

Accuracy = 0
for i in range(10):
    Accuracy += matrix[i, i]
Accuracy /= np.sum(matrix)

print("Accuracy:", Accuracy)
print("Confusion matrix: ")
print(matrix)