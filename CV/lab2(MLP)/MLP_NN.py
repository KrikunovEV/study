import numpy as np


class NeuralNetwork():

    def __init__(self, listNeurons=[1,1], listFuncs=[], Task='Regression', lr=0.001, lamb=0.001, iters=501, batch_size=64):
        self.listNeurons = listNeurons
        self.listFuncs = listFuncs
        self.Task = Task

        self.lr = lr
        self.lamb = lamb
        self.iters = iters
        self.batch_size = batch_size

        self.W = []
        self.bias = []


    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def tanh(self, x):
        return np.tanh(x)

    def relu(self, x):
        return np.maximum(0, x)

    def softmax(self, x):
        e = np.exp(x)
        return e / np.sum(e)


    def tanh_grad(self, x):
        return 1 - x ** 2

    def sigmoid_grad(self, x):
        return x * (1 - x)

    def relu_grad(self, x):
        x[x <= 0] = 0
        x[x > 0] = 1
        return x

    def softmax_grad(self, x, label):
        return x - label  # de/dy * dy/dz

    def regression_grad(self, x, label):
        return x - label  # de/dy * dy/dz


    def RegressionLoss(self, x, label):
        return np.square(x - label).sum() / 2 + np.sum(self.W[-1]**2) / 2 * self.lamb

    def SoftmaxLoss(self, x, label):
        y = np.log(self.softmax(x))
        return - np.sum(y * label) + np.sum(self.W[-1]**2) / 2 * self.lamb


    def train(self, inputs, labels):

        # INITIALIZE
        for i in range(1, len(self.listNeurons)):
            # multiplier 0.01 is OK for small net
            self.W.append(np.random.randn(self.listNeurons[i], self.listNeurons[i-1]) * 0.01)
            self.bias.append(np.random.randn(self.listNeurons[i]) * 0.01)

        Loss = []

        for iteration in range(1, self.iters):
            loss = 0

            batch = np.random.choice(len(inputs), self.batch_size)
            batch_inputs = inputs[batch]
            batch_labels = labels[batch]

            for input, label in zip(batch_inputs, batch_labels):
                if type(input) != np.ndarray:
                    input = np.array([input])

                # FORWARD
                layers_data = [input]
                for i, (w, b) in enumerate(zip(self.W, self.bias)):
                    h = w.dot(layers_data[-1]) + b

                    isLastLayer = (i == (len(self.W) - 1))
                    if not isLastLayer:
                        if self.listFuncs[i] == 'tanh':
                            h = self.tanh(h)
                        elif self.listFuncs[i] == 'sigmoid':
                            h = self.sigmoid(h)
                        elif self.listFuncs[i] == 'relu':
                            h = self.relu(h)
                    elif self.Task == 'Classification':
                        h = self.softmax(h)

                    layers_data.append(h)

                if self.Task == 'Classification':
                    loss += self.SoftmaxLoss(layers_data[-1], label)
                elif self.Task == 'Regression':
                    loss += self.RegressionLoss(layers_data[-1], label)


                # BACKWARD
                grad = []
                grad_w = []
                grad_b = []
                for i in range(len(self.W) - 1, -1, -1):

                    isLastLayer = (i == (len(self.W) - 1))

                    if isLastLayer:
                        if self.Task == 'Classification':
                            grad.append(self.softmax_grad(layers_data[-1], label))
                        elif self.Task == 'Regression':
                            grad.append(self.regression_grad(layers_data[-1], label))
                            #grad.append(np.ones(len(layers_data[-1])))

                        grad_w.append(grad[-1].reshape((len(grad[-1]), 1)).dot
                              (layers_data[-2].reshape((1, len(layers_data[-2])))) + self.lamb * self.W[-1])
                        grad_b.append(grad[-1])

                    else:
                        grad.append(self.W[i + 1].T.dot(grad[-1]))

                        gradient = 0
                        if self.listFuncs[i] == 'tanh':
                            gradient = self.tanh_grad(layers_data[i+1])
                        elif self.listFuncs[i] == 'sigmoid':
                            gradient = self.sigmoid_grad(layers_data[i+1])
                        elif self.listFuncs[i] == 'relu':
                            gradient = self.relu_grad(layers_data[i+1])

                        grad_b.append(grad[-1] * gradient)
                        grad_w.append(grad_b[-1].reshape((len(grad[-1]), 1)).dot
                                      (layers_data[i].reshape((1, len(layers_data[i])))))


                # OPTIMIZE
                for i in range(len(self.W) - 1, -1, -1):
                    j = len(self.W) - (i + 1)
                    self.W[i] -= grad_w[j] * self.lr
                    self.bias[i] -= grad_b[j] * self.lr

            if iteration % 500 == 0:
                self.lr *= 0.9
            Loss.append(loss)
            print('epoch:', iteration, '; batch loss:', loss)

        return Loss


    def Forward(self, input, label):

        if type(input) != np.ndarray:
            input = np.array([input])

        h_data = input
        for i, (w, b) in enumerate(zip(self.W, self.bias)):
            h_data = w.dot(h_data) + b
            if i != len(self.W) - 1:
                if self.listFuncs[i] == 'tanh':
                    h_data = self.tanh(h_data)
                elif self.listFuncs[i] == 'sigmoid':
                    h_data = self.sigmoid(h_data)
                elif self.listFuncs[i] == 'relu':
                    h_data = self.relu(h_data)
            else:
                if self.Task == "Classification":
                    h_data = self.softmax(h_data)
                    loss = self.SoftmaxLoss(h_data, label)
                elif self.Task == 'Regression':
                    loss = self.RegressionLoss(h_data, label)

        return h_data, loss