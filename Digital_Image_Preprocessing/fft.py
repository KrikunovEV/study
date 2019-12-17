import numpy as np
import matplotlib.pyplot as plt
import cv2 as cv


def DC(image):
    h, w = image.shape
    for y in range(h):
        for x in range(w):
            image[y, x] = image[y, x] if (x + y) % 2 == 0 else -image[y, x]
    return image


def Fourie(image):
    M, N = image.shape
    result_image = np.empty((M, N))

    for u in range(M):
        for v in range(N):

            print(image.shape, u, v)

            sum_m = 0
            for m in range(M):
                sum_n = 0
                for n in range(N):
                    sum_n += np.exp((-2 * np.pi * n * v * 1j) / N) * image[m, n]
                sum_m += sum_n / N * np.exp((-2 * np.pi * m * u * 1j) / M)

            result_image[u, v] = np.log(np.abs(sum_m / M))

    return result_image

image = np.asarray(cv.imread('emma.jpg', cv.IMREAD_GRAYSCALE), dtype=np.float32)[::50, ::50]

image = DC(image)
image = Fourie(image)
print(image)

plt.imshow(image, cmap='gray')
plt.show()
