# petrovets.for.students@gmail.com

import cv2 as cv
import numpy as np

image = cv.imread("segment.jpeg")
hsv_image = cv.cvtColor(image, cv.COLOR_BGR2HSV)


# Green
mask = cv.inRange(hsv_image, np.array([55, 0, 0]), np.array([75, 255, 200]))
contours, _ = cv.findContours(mask.copy(), cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
for contour in contours:
    x, y, w, h = cv.boundingRect(contour)
    if w * h >= 10000:  # ~100x100
        cv.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 5)

# Red
mask = cv.inRange(hsv_image, np.array([1, 110, 200]), np.array([10, 255, 255]))
contours, _ = cv.findContours(mask.copy(), cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
for contour in contours:
    x, y, w, h = cv.boundingRect(contour)
    if w * h >= 5000:  # ~50x100
        cv.rectangle(image, (x, y), (x + w, y + h), (0, 0, 255), 5)


image_resized = cv.resize(image, (1280, 720))
cv.imshow("Segmentation by color", image_resized)
cv.waitKey()
