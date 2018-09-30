import cv2
import numpy as np
import matplotlib.pyplot as plt


img = cv2.cvtColor(cv2.imread('empire.jpg'), cv2.COLOR_BGR2RGB)
plt.title('RGB image with shape (%d,%d,%d)' % img.shape)
plt.imshow(img)
plt.show()


img = np.float32(img) / 255.0 + np.random.randn(img.shape[0], img.shape[1], img.shape[2]) * 0.1
img[img > 1.0] = 1.0
img[img < 0.0] = 0.0
plt.title('Adding a noise to image')
plt.imshow(img)
plt.show()


img = np.uint8(np.mean(img, axis=2) * 255.0)
plt.title('Make it gray')
plt.imshow(img, cmap='gray')
plt.show()


bins = np.bincount(img.flatten())
plt.title('Image histogram')
plt.plot(np.arange(0, 256), bins)
plt.show()


img = cv2.copyMakeBorder(img, 4, 4, 4, 4, cv2.BORDER_REPLICATE)
filter = np.empty((9,9))
sigma = 2**2
for x in range(-4, 5):
    for y in range(-4, 5):
        filter[x+4,y+4] = np.exp(-(x*x+y*y)/(2*sigma))/(2*np.pi*sigma)
filter /= np.sum(filter)
for x in range(4, img.shape[0]-4):
    for y in range(4, img.shape[1]-4):
        img[x, y] = np.sum(img[x-4:x+5, y-4:y+5] * filter)
img = img[4:img.shape[0]-4, 4:img.shape[1]-4]
plt.title('Gaussian Blur with 9x9 filter & sgima = 2')
plt.imshow(img, cmap='gray')
plt.show()