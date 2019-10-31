import pickle
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torch.nn.functional as functional
import torch.optim as optim
import numpy as np
import cv2 as cv


BATCH_SIZE = 256
LR = 0.001
ITERATIONS = 2000
TRAIN = True
COLORS = ['r', 'g', 'b', 'm']
MODELS_ORDER = ['RGB', 'HSV', 'CMYK', 'LAB']


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
        )

    def forward(self, data, batch_size=0):
        data = self.Conv(data)
        if batch_size != 0:
            data = data.view(BATCH_SIZE, -1)
        else:
            data = data.flatten()
        data = self.FC(data)
        return data


def unpickle(file):
    with open(file, 'rb') as fo:
        data = pickle.load(fo, encoding='bytes')
    return data


def preprocess(img):
    for i in range(len(img)):
        img[i, 0] = (img[i, 0] - np.mean(img[i, 0])) / (np.std(img[i, 0]) + 0.0000000001)
        img[i, 1] = (img[i, 1] - np.mean(img[i, 1])) / (np.std(img[i, 1]) + 0.0000000001)
        img[i, 2] = (img[i, 2] - np.mean(img[i, 2])) / (np.std(img[i, 2]) + 0.0000000001)
    return img


def convert_color(img, color_space):
    img_converted = img.transpose((0, 2, 3, 1))  # N, 32, 32, 3
    for i in range(len(img_converted)):
        img_converted[i] = cv.cvtColor(img_converted[i], color_space)
    img_converted = img_converted.transpose((0, 3, 1, 2))  # N, 3, 32, 32
    return img_converted


def train(img, labels, save_path):
    Loss = nn.CrossEntropyLoss(reduction='sum')
    model = Model()
    optimizer = optim.Adam(model.parameters(), lr=LR)

    epoch_losses = []
    for _ in range(ITERATIONS):
        batch = np.random.choice(range(len(img)), BATCH_SIZE)
        prediction = model(torch.Tensor(img[batch]), BATCH_SIZE)
        loss = Loss(prediction, torch.LongTensor(labels[batch]))
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
        epoch_losses.append(loss.item())

    state = {'model': model.state_dict(), 'loss': epoch_losses}
    torch.save(state, save_path)


def accuracy(img, labels, model):
    correct = 0
    batch = np.random.choice(range(len(img)), BATCH_SIZE)
    prediction = model(torch.Tensor(img[batch]), BATCH_SIZE)
    probabilities = functional.softmax(prediction, dim=-1)
    for i, probs in enumerate(probabilities):
        if torch.argmax(probs) == labels[batch][i]:
            correct += 1
    return correct / BATCH_SIZE


data1 = unpickle('../../../cifar-10-batches-py/data_batch_1')
data2 = unpickle('../../../cifar-10-batches-py/data_batch_2')
data3 = unpickle('../../../cifar-10-batches-py/data_batch_3')
data4 = unpickle('../../../cifar-10-batches-py/data_batch_4')
data5 = unpickle('../../../cifar-10-batches-py/data_batch_5')
data_test = unpickle('../../../cifar-10-batches-py/test_batch')

img_all = np.vstack((data1[b'data'].reshape((len(data1[b'data']), 3, 32, 32)),
                     data2[b'data'].reshape((len(data2[b'data']), 3, 32, 32)),
                     data3[b'data'].reshape((len(data3[b'data']), 3, 32, 32)),
                     data4[b'data'].reshape((len(data4[b'data']), 3, 32, 32)),
                     data5[b'data'].reshape((len(data5[b'data']), 3, 32, 32)),
                     data_test[b'data'].reshape((len(data_test[b'data']), 3, 32, 32))))
labels = np.hstack((np.array(data1[b'labels']),
                    np.array(data2[b'labels']),
                    np.array(data3[b'labels']),
                    np.array(data4[b'labels']),
                    np.array(data5[b'labels'])))
labels_test = np.array(data_test[b'labels'])


img_rgb = preprocess(np.asarray(img_all, dtype=np.float64))
img_rgb_test = img_rgb[-len(labels_test):]
img_rgb = img_rgb[:-len(labels_test)]

img_hsv = convert_color(img_all, cv.COLOR_RGB2HSV)
img_hsv = preprocess(np.asarray(img_hsv, dtype=np.float64))
img_hsv_test = img_hsv[-len(labels_test):]
img_hsv = img_hsv[:-len(labels_test)]

img_cmyk = convert_color(img_all, cv.COLOR_RGB2YCR_CB)
img_cmyk = preprocess(np.asarray(img_cmyk, dtype=np.float64))
img_cmyk_test = img_cmyk[-len(labels_test):]
img_cmyk = img_cmyk[:-len(labels_test)]

img_lab = convert_color(img_all, cv.COLOR_RGB2LAB)
img_lab = preprocess(np.asarray(img_lab, dtype=np.float64))
img_lab_test = img_lab[-len(labels_test):]
img_lab = img_lab[:-len(labels_test)]

if TRAIN:
    train(img_rgb, labels, 'models/model_rgb.pt')
    train(img_hsv, labels, 'models/model_hsv.pt')
    train(img_cmyk, labels, 'models/model_cmyk.pt')
    train(img_cmyk, labels, 'models/model_lab.pt')

states = []
states.append(torch.load('models/model_rgb.pt'))
states.append(torch.load('models/model_hsv.pt'))
states.append(torch.load('models/model_cmyk.pt'))
states.append(torch.load('models/model_lab.pt'))

# TRAIN LOSSES
fig, axs = plt.subplots(1, 2)
ax_loss, ax_accuracy = axs[0], axs[1]
ax_loss.set_title('Train losses')
ax_loss.set_xlabel('iteration')
ax_loss.set_ylabel('epoch loss')
for i, state in enumerate(states):
    ax_loss.plot(range(len(state['loss'])), state['loss'], COLORS[i], label=MODELS_ORDER[i])
ax_loss.legend(loc=1)

# ACCURACY
model_rgb = Model()
model_rgb.load_state_dict(states[0]['model'])
model_hsv = Model()
model_hsv.load_state_dict(states[1]['model'])
model_cmyk = Model()
model_cmyk.load_state_dict(states[2]['model'])
model_lab = Model()
model_lab.load_state_dict(states[3]['model'])

train_acc = []
train_acc.append(accuracy(img_rgb, labels, model_rgb))
train_acc.append(accuracy(img_hsv, labels, model_hsv))
train_acc.append(accuracy(img_cmyk, labels, model_cmyk))
train_acc.append(accuracy(img_lab, labels, model_lab))

test_acc = []
test_acc.append(accuracy(img_rgb_test, labels_test, model_rgb))
test_acc.append(accuracy(img_hsv_test, labels_test, model_hsv))
test_acc.append(accuracy(img_cmyk_test, labels_test, model_cmyk))
test_acc.append(accuracy(img_lab_test, labels_test, model_lab))

ind = np.arange(len(MODELS_ORDER))
width = 0.35

ax_accuracy.bar(ind - width/2, train_acc, width, label='train accuracy')
ax_accuracy.bar(ind + width/2, test_acc, width, label='test accuracy')
ax_accuracy.set_title('Accuracy on train and test')
ax_accuracy.set_ylabel('accuracy')
ax_accuracy.set_xticks(ind)
ax_accuracy.set_xticklabels(MODELS_ORDER)
ax_accuracy.legend()

plt.show()
