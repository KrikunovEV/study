import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal
import time


image = cv.imread("board.jpeg", cv.IMREAD_GRAYSCALE)#[::2, ::2]

start_time = time.time()

filter = np.array([[1, 0, -1],
                   [2, 0, -2],
                   [1, 0, -1]])
Ix = scipy.signal.convolve2d(image, filter, 'same')

filter = filter.T
Iy = scipy.signal.convolve2d(image, filter, 'same')

Ix2 = Ix ** 2
Iy2 = Iy ** 2
Ixy = Ix * Iy

window = np.ones((5, 5))
Sx = scipy.signal.convolve2d(Ix2, window, 'same')
Sy = scipy.signal.convolve2d(Iy2, window, 'same')
Sxy = scipy.signal.convolve2d(Ixy, window, 'same')

det = Sx * Sy - Sxy ** 2
trace = Sx + Sy

responses = det - 0.04 * trace ** 2

threshold = 0.5
image = np.array([image, image, image])
image = np.transpose(image, (1, 2, 0))
image[responses >= threshold * np.max(responses)] = np.array([0, 0, 255])
#image = cv.resize(image, (816, 612))

print(time.time() - start_time)

cv.imshow("Harris", image)
cv.waitKey()