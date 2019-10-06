# petrovets.for.students@gmail.com

import cv2 as cv
import matplotlib.pyplot as plt

image = cv.imread("segment.jpeg")
plt.imshow(image)

hsv_image = cv.cvtColor(image, cv.COLOR_BGR2HSV)

h = 145
s = range(256)
v = range(256)

mask = hsv_image[:, :, 0] > h
hsv_image[mask] = 255

plt.imshow(hsv_image)
plt.show()

cv.waitKey(0)