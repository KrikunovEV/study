import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

'''
cv, np, plt

class data loader CIFAR10

constructor (filepath, type, shuffle, batch_size, epoch_size)
print_statistics (кол-во элементов, классов и элементов в каждом классе)
batch_generator (preprocess, augment and return batch)
sample (probability for each class)
preprocess (resize, normalize)
one-hot encoding (get one-hot vector for label of image)
data augmentation (regularization)
    crop, noise, rotation, scaling, brightness, hue, saturation, contrast, blur, pad, flip

vizualization batch on each epoch
'''
