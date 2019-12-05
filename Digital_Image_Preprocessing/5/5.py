import numpy as np
import matplotlib.pyplot as plt
import cv2 as cv
import scipy.signal as sp


def gaussian_filter(image):

    filter = np.empty((5, 5))
    sigma = 1.4
    for x in range(-2, 3):
        for y in range(-2, 3):
            filter[y + 2, x + 2] = (1 / 2 * np.pi * sigma**2) * np.exp(-(x**2 + y**2) / (2 * sigma**2))
    filter /= np.sum(filter)

    return sp.convolve2d(image, filter, 'same')


def sobel_filter(image):

    filterx = np.array([[-1, 0, 1],
                        [-2, 0, 2],
                        [-1, 0, 1]])
    filtery = np.array([[ 1,  2,  1],
                        [ 0,  0,  0],
                        [-1, -2, -1]])
    imagex = sp.convolve2d(image, filterx, 'same')
    imagey = sp.convolve2d(image, filtery, 'same')

    modules = np.sqrt(imagex**2 + imagey**2)
    directions = np.arctan(imagey/(imagex + 0.00000000001))

    return modules, directions


def NMS(modules, directions):
    mask = np.zeros_like(modules, np.bool)
    eps = (np.pi / 4) / 2

    for y in range(len(modules)):
        for x in range(len(modules[0])):
            theta = directions[y, x]
            if -eps <= theta <= eps or -np.pi <= theta <= (-np.pi + eps) or (np.pi - eps) <= theta <= np.pi:
                # horizontal
                x_ind = [i for i in range(x - 1, x + 2) if 0 <= i <= (len(mask[0]) - 1)]
                if x_ind[np.argmax(modules[y, x_ind])] != x:
                    mask[y, x] = True
            elif (np.pi / 2) - eps <= theta <= (np.pi / 2) + eps or (-np.pi / 2) - eps <= theta <= (-np.pi / 2) + eps:
                # vertical
                y_ind = [i for i in range(y - 1, y + 2) if 0 <= i <= (len(mask) - 1)]
                if y_ind[np.argmax(modules[y_ind, x])] != y:
                    mask[y, x] = True
            elif (np.pi / 4) - eps <= theta <= (np.pi / 4) + eps or (-3 * np.pi / 2) - eps <= theta <= (-3 * np.pi / 2) + eps:
                # main diag
                x_ind = [(x + i) for i in range(-1, 2) if
                         0 <= (x + i) <= (len(mask[0]) - 1) and 0 <= (y + i) <= (len(mask) - 1)]
                y_ind = [(y + i) for i in range(-1, 2) if
                         0 <= (x + i) <= (len(mask[0]) - 1) and 0 <= (y + i) <= (len(mask) - 1)]
                if y_ind[np.argmax(modules[(y_ind, x_ind)])] != y:
                    mask[y, x] = True
            else:
                # secondary diag
                x_ind = [(x + i) for i in range(-1, 2) if
                         0 <= (x + i) <= (len(mask[0]) - 1) and 0 <= (y - i) <= (len(mask) - 1)]
                y_ind = [(y - i) for i in range(-1, 2) if
                         0 <= (x + i) <= (len(mask[0]) - 1) and 0 <= (y - i) <= (len(mask) - 1)]
                if y_ind[np.argmax(modules[(y_ind, x_ind)])] != y:
                    mask[y, x] = True

    modules[mask] = 0
    return modules


def canny_edge(modules):
    low_threshold, high_threshold = 0, 0


image = cv.imread('planes.jpg', cv.IMREAD_GRAYSCALE)
image = gaussian_filter(image)
modules, directions = sobel_filter(image)
modules = NMS(modules, directions)
canny_edge(modules)

plt.imshow(np.asarray((modules / np.max(modules)) * 255, np.uint8), cmap='gray')
plt.show()

