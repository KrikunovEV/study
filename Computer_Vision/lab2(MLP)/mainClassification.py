from Dataset import *
from MLP_NN import *
import matplotlib.pyplot as plt


trainData, trainLabel, validData, validLabel, testData, testLabel = GetDataForClassification()



# TRAINING

num_input = len(trainData[0])
num_k = 10

lr = [0.0001, 0.001]
batch_size = [32, 64]
iters = [251, 501]
num_neurons = [32, 64, 128]
num_layers = [1, 2]
funcs = ['relu', 'tanh']
lambs = [0.0, 0.001]

for i in range(len(lr)):
    for j in range(len(batch_size)):
        for q in range(len(iters)):
            for w in range(len(num_neurons)):
                for e in range(len(num_layers)):
                    for t in range(len(funcs)):
                        for y in range(len(lambs)):

                            listNeurons = [num_input]
                            listFuncs = []
                            for n in range(num_layers[e]):
                                listNeurons.append(num_neurons[w])
                                listFuncs.append(funcs[t])
                            listNeurons.append(num_k)

                            brain = NeuralNetwork(
                                listNeurons=listNeurons,
                                listFuncs=listFuncs,
                                Task='Classification',
                                lr=lr[i],
                                lamb=lambs[y],
                                iters=iters[q],
                                batch_size=batch_size[j])

                            Losses = brain.train(trainData, trainLabel)

                            pickle_it(brain, "Models/%s_%s_%s_%s_%s_%s_%s.pickle" %
                                      (lr[i], batch_size[j], iters[q], num_neurons[w], num_layers[e],funcs[t], lambs[y]))



# VALIDATION

Accuracy = []
brain = []
params = []

for i in range(len(lr)):
    for j in range(len(batch_size)):
        for q in range(len(iters)):
            for w in range(len(num_neurons)):
                for e in range(len(num_layers)):
                    for t in range(len(funcs)):
                        for y in range(len(lambs)):

                            params.append("lr: %s\nbatch_size: %s\niters: %s\nnum_neurons: %s\nnum_layers: %s\nfuncs: %s\nlamb: %s" %
                                      (lr[i], batch_size[j], iters[q], num_neurons[w], num_layers[e],funcs[t], lambs[y]))
                            brain.append(unpickle_it("Models/%s_%s_%s_%s_%s_%s_%s.pickle" %
                                      (lr[i], batch_size[j], iters[q], num_neurons[w], num_layers[e],funcs[t], lambs[y])))

                            matrix = np.zeros((num_k, num_k))

                            for a in range(len(validData)):
                                predict, _ = brain[-1].Forward(validData[a], validLabel[a])
                                matrix[np.argmax(validLabel[a]), np.argmax(predict)] += 1

                            diag = 0
                            for k in range(num_k):
                                diag += matrix[k, k]

                            Accuracy.append(diag / np.sum(matrix))


THE_BEST_BRAIN = np.argmax(Accuracy)
THE_WORST_BRAIN = np.argmin(Accuracy)
ConfusionMatrix = np.zeros((num_k, num_k))

for input, label in zip(testData, testLabel):
    predict, _ = brain[THE_BEST_BRAIN].Forward(input, label)
    ConfusionMatrix[np.argmax(label), np.argmax(predict)] += 1

diag = 0
for k in range(num_k):
    diag += ConfusionMatrix[k, k]

Accuracy = diag / np.sum(ConfusionMatrix)

print('\nTHE BEST BRAIN:\nAccuracy is', Accuracy)
print(params[THE_BEST_BRAIN])
print('Confusion matrix:')
print(ConfusionMatrix)
print('\n\nParams of the worst brain: ')
print(params[THE_WORST_BRAIN])