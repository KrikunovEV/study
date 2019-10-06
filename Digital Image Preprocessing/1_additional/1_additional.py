import pickle
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np


BATCH_SIZE = 256
LR = 0.0001
EPOCH = 500


class Model(nn.Module):

    def __init__(self):
        super(Model, self).__init__()

        self.Conv = nn.Sequential(
            nn.Conv2d(3, 32, 3),
            nn.ReLU(),
            nn.MaxPool2d(2),
            nn.Conv2d(32, 64, 3),
            nn.ReLU(),
            nn.MaxPool2d(2),
            nn.Conv2d(64, 128, 3),
            nn.ReLU(),
            nn.MaxPool2d(2),
        )

        self.FC = nn.Sequential(
            nn.Linear(128 * 2 * 2, 128),
            nn.ReLU(),
            nn.Linear(128, 10),
            #nn.Softmax(dim=-1)
        )

    def forward(self, data):
        data = self.Conv(data)
        data = data.view(BATCH_SIZE, -1)
        data = self.FC(data)
        return data


def unpickle(file):
    with open(file, 'rb') as fo:
        data = pickle.load(fo, encoding='bytes')
    return data


data = unpickle('../../../cifar-10-batches-py/data_batch_1')
img = data[b'data'].reshape((10000, 3, 32, 32))  # .transpose(0, 2, 3, 1)  # 10000, 32, 32, 3
labels = np.array(data[b'labels'])

model = Model()
optimizer = optim.Adam(model.parameters(), lr=LR)
Loss = nn.CrossEntropyLoss(reduction='sum')

batch_losses = []

for epoch in range(EPOCH):
    ind = np.random.choice(range(10000), BATCH_SIZE)
    prediction = model(torch.Tensor(img[ind]))
    batch_loss = Loss(prediction, torch.LongTensor(labels[ind]))
    optimizer.zero_grad()
    batch_loss.backward()
    optimizer.step()

    batch_losses.append(batch_loss.item())

plt.plot(range(len(batch_losses)), batch_losses)
plt.show()
