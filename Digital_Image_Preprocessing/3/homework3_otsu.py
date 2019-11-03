import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

image = cv.imread("image2.jpg", cv.IMREAD_GRAYSCALE)

hist = np.histogram(image, 256)[0]
hist = hist / np.sum(hist)

count = 0
sum = 0

sum_common = np.sum(np.arange(256) * hist)

Losses = []

for i in range(256):
    count += hist[i]
    count2 = 1 - count

    if count == 0 or count2 == 0:
        Losses.append(0)
        continue

    sum += i * hist[i]
    mean = sum / count
    mean2 = (sum_common - sum) / count2

    Losses.append(count * count2 * (mean - mean2)**2)

T = np.argmax(Losses)
th, cv_image = cv.threshold(image, 0, 255, cv.THRESH_OTSU)
print("My threshold: ", T)
print("CV's threshold: ", th)

ax = plt.subplot(121)
ax.set_title('Original image')
ax.imshow(image, cmap='gray')

ax = plt.subplot(122)
image[image <= T] = 0
image[image > T] = 255
ax.set_title('Otsu\'s binarization (T = ' + str(T) + ")")
ax.imshow(image, cmap='gray')

#ax = plt.subplot(212)
#ax.set_title('Otsu losses')
#ax.plot(np.arange(256), Losses, 'b')


plt.show()
