import torch
import time
import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import fetch_mldata


mnist = fetch_mldata('MNIST original', data_home="./")
all_data = np.array(mnist.data, dtype=np.float) / 255.0
all_label = mnist.target

k = 0
for i in range(1, len(all_label)):
    if all_label[i-1] == 9.0 and all_label[i] == 0.0:
        k = i
        break

trainData = torch.Tensor(all_data[:k])
trainLabel = torch.LongTensor(all_label[:k])
testData = torch.Tensor(all_data[k:])
testLabel = torch.LongTensor(all_label[k:])

model = torch.nn.Sequential(
    torch.nn.Linear(28 * 28, 512),
    torch.nn.ReLU(),
    torch.nn.Linear(512, 256),
    torch.nn.ReLU(),
    torch.nn.Linear(256, 10),
    torch.nn.Softmax(dim=1)
)

loss_fn = torch.nn.CrossEntropyLoss(reduction="sum")

optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

Loss = []
start_time = time.time()

for iteration in range(2000):

    batch = np.random.choice(len(trainData), 256)
    input = trainData[batch]
    label = trainLabel[batch]

    pred = model(input)

    loss = loss_fn(pred, label)
    print(iteration, loss.item())
    Loss.append(loss.item())

    optimizer.zero_grad()
    loss.backward()
    optimizer.step()

    if iteration % 250 == 0 and iteration != 0:
        for group in range(len(optimizer.param_groups)):
            optimizer.param_groups[group]['lr'] *= 0.9

print("--- %s seconds ---" % (time.time() - start_time))

pred = model(testData)
Matrix = np.zeros((10, 10))
Accuracy = 0

for i, p in enumerate(pred):
    a = np.argmax(p.detach().numpy())
    Matrix[a, testLabel[i]] += 1
    if a == testLabel[i]:
        Accuracy += 1

print("Accuracy is", Accuracy / len(testData))

plt.title('Batch losses')
plt.plot(np.arange(len(Loss)), Loss)
plt.show()
