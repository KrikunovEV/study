import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt


def get_integral_image(image):
    integral_image = np.asarray(image, dtype='object')
    for x in range(1, len(integral_image[0])):
        integral_image[0, x] += integral_image[0, x - 1]
    for y in range(1, len(integral_image)):
        integral_image[y, 0] += integral_image[y - 1, 0]

    for y in range(1, len(integral_image)):
        for x in range(1, len(integral_image[0])):
            integral_image[y, x] = integral_image[y, x] - integral_image[y - 1, x - 1] + integral_image[y, x - 1] + \
                                   integral_image[y - 1, x]

    return integral_image


image = cv.imread("image2.jpg", cv.IMREAD_GRAYSCALE)
integral_image = get_integral_image(image)
integral_image2 = get_integral_image(image**2)

w = 20
w2 = w / 2

image_bin = np.copy(image)
max_int = np.max(image)

for y in range(1, len(integral_image)):
    for x in range(1, len(integral_image[0])):

        x0, y0 = int(x - w2), int(y - w2)
        x1, y1 = int(x + w2), int(y + w2)

        if x0 < 0:
            x0 = 0
            x1 = w
        elif x1 >= len(image[0]):
            x1 = len(image[0]) - 1
            x0 = x1 - w

        if y0 < 0:
            y0 = 0
            y1 = w
        elif y1 >= len(image):
            y1 = len(image) - 1
            y0 = y1 - w

        S1 = (integral_image[y1, x1] + integral_image[y0, x0] - integral_image[y1, x0] - integral_image[y0, x1]) / w**2
        S2 = (integral_image2[y1, x1] + integral_image2[y0, x0] - integral_image2[y1, x0] - integral_image2[y0,
                                                                                                            x1]) / w**2

        mean = S1
        var = S2 / w**2 - (S1 / (w**2))**2

        T = mean * (1 + 0.05 * (var / 128 - 1))

        image_bin[y, x] = 255 if image_bin[y, x] > T else 0

        #print(mean, var, T)


(fig, ax) = plt.subplots(1, 2)
ax[0].set_title('Original')
ax[0].imshow(image, cmap="gray")
ax[1].set_title('Sauvola\'s binarization')
ax[1].imshow(image_bin, cmap="gray")
plt.show()