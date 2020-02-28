import numpy as np
import cv2 as cv
from scipy import signal
import matplotlib.pyplot as plt


def preproc_sobel(image):

    Gx = np.array([[-1, 0, 1],
                   [-2, 0, 2],
                   [-1, 0, 1]])

    Gy = np.array([[-1, -2, -1],
                   [ 0,  0,  0],
                   [ 1,  2,  1]])

    theta = np.deg2rad(10)
    pi2 = np.pi / 2

    imagex = signal.convolve2d(image, Gx, 'same') + 0.000000001
    imagey = signal.convolve2d(image, Gy, 'same')
    radians = np.arctan(imagey / imagex)

    mask1 = np.ma.getmask(np.ma.masked_inside(radians, 0, theta))
    mask2 = np.ma.getmask(np.ma.masked_inside(radians, pi2 - theta, pi2 + theta))
    mask3 = np.ma.getmask(np.ma.masked_inside(radians, np.pi - theta, np.pi))
    mask = mask1 + mask2 + mask3
    image[mask == False] = 0

    return image


def second_order_difference(image):
    filter = np.array([[-1],
                       [ 2],
                       [-1]])

    return np.abs(signal.convolve2d(image, filter, 'same')), np.abs(signal.convolve2d(image, filter.T, 'same'))


def first_improvement(Dy, Dx):
    filter = np.ones((1, 32))
    Esy = signal.convolve2d(Dy, filter, 'same')
    Ey = Esy.copy()

    for y in range(len(Esy)):
        for x in range(len(Esy[0])):
            mid = []
            if y < 16:
                for i in range(16 - y):
                    mid.append(0)
                for i in range(32 - (16-y)):
                    mid.append(Esy[i, x])
            elif y > len(Esy) - 16:
                for i in range(y, len(Esy) - y):
                    mid.append(Esy[i, x])
                for i in range(16 - (len(Esy) - y)):
                    mid.append(0)
            else:
                mid = Esy[(y-16):(y+16), x]
            Ey[y, x] -= np.median(mid)

    Esx = signal.convolve2d(Dx, filter.T, 'same')
    Ex = Esx.copy()

    for y in range(len(Esx)):
        for x in range(len(Esx[0])):
            mid = []
            if x < 16:
                for i in range(16 - x):
                    mid.append(0)
                for i in range(32 - (16 - x)):
                    mid.append(Esx[y, i])
            elif x > len(Esx[0]) - 16:
                for i in range(x, len(Esx[0]) - x):
                    mid.append(Esx[y, i])
                for i in range(16 - (len(Esx[0]) - x)):
                    mid.append(0)
            else:
                mid = Esx[y, (x - 16):(x + 16)]
            Ex[y, x] -= np.median(mid)

    return Ey, Ex


def second_improvement(Ey, Ex):
    G = np.asarray(Ey)
    for y in range(len(Ey)):
        for x in range(len(Ey[0])):
            y_ind = np.array([y - 16, y - 8, y, y + 8, y + 16])
            y_ind[y_ind < 0] = 0
            y_ind[y_ind > (len(Ey) - 1)] = (len(Ey) - 1)

            x_ind = np.array([x - 16, x - 8, x, x + 8, x + 16])
            x_ind[x_ind < 0] = 0
            x_ind[x_ind > (len(Ex[0]) - 1)] = len(Ex[0]) - 1

            G[y, x] = np.median(Ey[y_ind, x]) + np.median(Ex[y, x_ind])
    return G


def anomaly_score(G):

    B = np.zeros_like(G)

    blockx = int(len(G[0]) / 8)
    blocky = int(len(G) / 8)

    for by in range(blocky):
        for bx in range(blockx):
            x, y = bx * 8, by * 8
            A = G[y+1:y+7, x+1:x+7]
            miny = np.min(np.sum(image[(y, y+7), x+1:x+7], axis=1))
            minx = np.min(np.sum(image[y+1:y+7, (x, x+7)], axis=0))
            b = np.max(np.sum(A, axis=0)) + np.max(np.sum(A, axis=1)) - miny - minx
            B[y:y+8, x:x+8] = b

    return B


image = cv.imread('poster_.jpg', cv.IMREAD_GRAYSCALE)

#image = preproc_sobel(image)

Dy, Dx = second_order_difference(image)
Ey, Ex = first_improvement(Dy, Dx)
G = second_improvement(Ey, Ex)
B = anomaly_score(G)
#print(B)

plt.imshow(B, cmap='gray')
plt.show()
