'''

Sobel's filters:
-1 0 1     1  2  1
-2 0 2     0  0  0
-1 0 1    -1 -2 -1


      ->  dImg / dx (1st filter)         sqrt(x**2 + y**2) (карта модулей)
Img(gray)                                ->
      -> dImg / dy (2nd filter)          arctg (y / x) (карта модулей направлений)


->    с порогом Т = 200 Edge binary. (Посмотреть процедуру cv2.Canny(img, 100, 200))


->  Hough Transform
В полярных координатах r = x Cos(phi) + y Sin (phi)
x/y берём с Edge binary, phi дискретизируем (phi_min(-P/2), phi_max(P)) и находим r
                           r дискретизируем от 0 до sqrt(w**2 + h**2)

Аккулятируем в массив +1 в r и phi


После делаем non-maximum suppression с каким-то офсетом.

Переводим обратно в y = kx + b и рисуем линию cv2.line()



Оптимизация:
1) Сэмплим берём 70% точек.
2) Берём две точки рандомно у них уже есть свой phi и r -> аккумулируем
3) Gradient direction  -> phi


'''

import numpy as np
import matplotlib.pyplot as plt
import cv2

img = cv2.cvtColor(cv2.imread('empire.jpg'), cv2.COLOR_BGR2RGB)
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

rho_max = np.sqrt(img.shape[0]**2 + img.shape[1]**2)
rho_list = np.linspace(0, rho_max, 100)
phi_list = np.linspace(-np.pi/2, np.pi, 100)

Accumulator = np.zeros((100, 100))

for y in range(img.shape[0]):
    for x in range(img.shape[1]):
        if img[y, x] == 0:
            continue

        for phi_ind, phi in enumerate(phi_list):
            rho = x * np.cos(phi) + y * np.sin(phi)
            rho_ind = int((rho / rho_max) * 100)
            Accumulator[rho_ind, phi_ind] += 1



for rho_ind in range(100):
    for phi_ind in range(100):
        if Accumulator[rho_ind, phi_ind] == 0:
            continue

        # y * Sin(phi) = rho - x Cos(phi)
        # y = (rho - x Cos(phi)) / sin(phi)
        y1 = int((rho_list[rho_ind] + 100 * np.cos(phi_list[phi_ind])) / (np.sin(phi_list[phi_ind])+0.00000001))
        y2 = int((rho_list[rho_ind] - 100 * np.cos(phi_list[phi_ind])) / (np.sin(phi_list[phi_ind])+0.00000001))

        print(y1, y2)

        cv2.line(IMG, (-100, y1), (100, y2), (0, 255, 0), 1)

plt.title('Lines')
plt.imshow(IMG, cmap='gray')
plt.show()