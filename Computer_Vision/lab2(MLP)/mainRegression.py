from Dataset import *
from MLP_NN import *
import matplotlib.pyplot as plt

trainData, trainLabel, testData, testLabel = GetDataForRegression(5000)

brain = NeuralNetwork(
    listNeurons=[2, 4, 1],
    listFuncs=['relu'],
    Task='Regression',
    lr=0.001,
    lamb=0.001,
    iters=150,
    batch_size=256
)

Loss = brain.train(trainData, trainLabel)

LossF = 0
Predicts = []
for input, label in zip(testData, testLabel):
    predict, loss = brain.Forward(input, label)
    Predicts.append(predict)
    LossF += loss

plt.title("Batch loss on train Data")
plt.plot(np.arange(len(Loss)), Loss)
plt.show()

plt.title("Blue: expected; Red: predicted on test Data")
plt.plot(testData[:, 0], testLabel, c='b')
plt.plot(testData[:, 0], Predicts, c='r')
plt.show()