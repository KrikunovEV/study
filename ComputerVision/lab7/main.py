import numpy as np
import matplotlib.pyplot as plt
import cv2

img = cv2.cvtColor(cv2.imread('go_board.jpg'), cv2.COLOR_BGR2RGB)
plt.title('RGB image with shape (%d,%d,%d)' % img.shape)
plt.imshow(img)
plt.show()

IMG = img

img = np.float32(np.mean(img, axis=2) / 255.0)
plt.title('Gray image')
plt.imshow(img, cmap='gray')
plt.show()

Fx = np.array([
    [-1, 0, 1],
    [-2, 0, 2],
    [-1, 0, 1]
])
Fy = np.array([
    [1, 2, 1],
    [0, 0, 0],
    [-1, -2, -1]
])

img = np.sqrt(cv2.filter2D(img, -1, Fx)**2 + cv2.filter2D(img, -1, Fy)**2)
plt.title('Edge image')
plt.imshow(img, cmap='gray')
plt.show()

img = np.uint8(img * 255.0)
img[img >= 200.0] = 255
img[img < 200.0] = 0
plt.title('Edge binary image')
plt.imshow(img, cmap='gray')
plt.show()




rho_num = 300
phi_num = 300

rho_max = np.sqrt(img.shape[0]**2 + img.shape[1]**2)
rho_list = np.linspace(0, rho_max, rho_num)
phi_list = np.linspace(-np.pi/2, np.pi, phi_num)

Accumulator = np.zeros((rho_num, phi_num))

for y in range(img.shape[0]):
    for x in range(img.shape[1]):
        if img[y, x] == 0:
            continue

        for phi_ind, phi in enumerate(phi_list):
            rho = x * np.cos(phi) + y * np.sin(phi)
            if rho < 0:
                continue
            rho_ind = int((rho / rho_max) * rho_num)
            Accumulator[rho_ind, phi_ind] += 1

plt.title('Accumulator')
plt.imshow(Accumulator, cmap='gray')
plt.show()

# Non-maximum suppression
size = 50 #window
for partx in range(int(rho_num / size)):
    for party in range(int(phi_num / size)):
        x1 = partx * size
        x2 = (partx + 1) * size
        y1 = party * size
        y2 = (party + 1) * size

        ind = np.unravel_index(np.argmax(Accumulator[x1:x2, y1:y2], axis=None), Accumulator[x1:x2, y1:y2].shape)
        ind = [ind[0] + partx * size, ind[1] + party * size]
        value = Accumulator[ind[0], ind[1]]

        Accumulator[x1:x2, y1:y2] = 0
        Accumulator[ind[0], ind[1]] = value

        print(ind, Accumulator[ind[0], ind[1]])

plt.title('NMS Accumulator')
plt.imshow(Accumulator, cmap='gray')
plt.show()





for rho_ind in range(rho_num):
    for phi_ind in range(phi_num):
        if Accumulator[rho_ind, phi_ind] == 0:
            continue

        x1 = int(rho_list[rho_ind] / (np.cos(phi_list[phi_ind]) + 0.00000001))
        y1 = 0

        x2 = 0
        y2 = int(rho_list[rho_ind] / (np.sin(phi_list[phi_ind]) + 0.00000001))

        # Отсекаем линии параллельные к осям
        if abs(x1) > 1000000 or abs(y2) > 1000000:
            print('x1 or y2 is high')
            continue

        cv2.line(IMG, (x1, y1), (x2, y2), (0, 255, 0), 1)

plt.title('Lines')
plt.imshow(IMG, cmap='gray')
plt.show()