import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt


image = cv.imread("image20.png", cv.IMREAD_GRAYSCALE)

hist_orig = np.histogram(image, 255)[0]
hist = hist_orig / np.sum(hist_orig)
#plt.plot(range(255), hist)

f = [0]
for i in range(len(hist)):
    f.append(f[i] + hist[i])
f = np.array(f)[1:] * 255
print(f)
#plt.plot(range(255), f)

for value, old_value in zip(f, hist_orig):
    image[image == old_value] = int(value)

cv.imwrite("img.png", image)

plt.imshow(image, cmap='gray')
plt.show()