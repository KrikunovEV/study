import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

image = cv.imread("image20.png", cv.IMREAD_GRAYSCALE)

hist_orig = np.histogram(image, 256, (0, 255))[0]
hist = np.asarray(hist_orig, dtype=np.float32) / np.sum(hist_orig)

f = [0]
for i in range(len(hist)):
    f.append(f[i] + hist[i])
f = np.asarray(np.array(f)[1:] * 255, dtype=np.uint8)
f = f[::-1]

for i, value in enumerate(f):
    image[image == (255 - i)] = value

cv.imwrite("img.png", image)
plt.imshow(image, cmap='gray')
plt.show()