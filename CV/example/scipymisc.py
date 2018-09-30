from scipy import misc
import numpy as np
import matplotlib.pyplot as plt

path = 'CV-2018/Materials/go_board.jpg'
img = misc.imread(path)

print(img.shape)
print(img.dtype) # np.uint8, need np.float32
img = np.float32(img)
print(img.dtype) # np.float32

#to grayscale
gray = np.mean(img, axis=2)
gray = np.uint8(gray) # recommended

#to binary
T = 128
bin = gray > T
print(bin.dtype) # np.Boolean
bin = 255 * np.uint8(bin)
print(bin.dtype) # np.uint8


# gamma correction
G = (np.uint8(gray) / 255) ** 0.05 # /255 - normalize, <1 lighter, >1 drarker

# negative
N = 255 - gray # warning !!!  may be wrong

# NOISE
noise = np.random.randn(img.shape)
moise_img = img + noise # dont forget about values in img

# Gauss blurring
# Image & kernel ->(conv)-> new blurring image
# kernel is filled by G formula in text file
# sigma is equals ???

# DZ
# myself KERNEL + blur


channel = img[:,:,0] # red, 1 - green, 2 - blue


plt.figure()
plt.imshow(img, cmap='gray')
plt.show()

misc.imsave("myimg.jpg", img)
