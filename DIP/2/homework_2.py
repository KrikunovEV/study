import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt


def get_lookup_table(histogram):
    CLIP = 0.5

    # normalize
    q = np.max(histogram)
    histogram = histogram / q

    # CLIP
    ind = histogram > CLIP
    value_for_each_bin = np.sum(histogram[ind] - CLIP) / 256
    histogram[ind] = CLIP
    histogram = (histogram + value_for_each_bin) * q

    # CDF
    hist = histogram / np.sum(histogram)
    f = [0]
    for i in range(len(hist)):
        f.append(f[i] + hist[i])
    return np.asarray(np.array(f)[1:] * 255, dtype=np.uint8)


CELLS_IN_WIDTH = 8
CELLS_IN_HEIGHT = 8

image = cv.imread("image.png", cv.IMREAD_GRAYSCALE)

cell_width, cell_height = int(len(image[0]) / CELLS_IN_WIDTH), int(len(image) / CELLS_IN_HEIGHT)
lookup_tables = np.empty(shape=(CELLS_IN_HEIGHT, CELLS_IN_WIDTH), dtype=np.ndarray)

#fig, axs = plt.subplots(8, 8)
for i in range(CELLS_IN_HEIGHT):
    for j in range(CELLS_IN_WIDTH):
        y0, y1 = i * cell_height, (i + 1) * cell_height
        x0, x1 = j * cell_width, (j + 1) * cell_width
        image_part = image[y0:y1, x0:x1]
        histogram = np.histogram(image_part, 256, (0, 255))[0]
        lookup_tables[i, j] = get_lookup_table(histogram)
        #axs[i][j].plot(range(256),  lookup_tables[i, j])
#plt.show()

for y in range(len(image)):
    for x in range(len(image[0])):

        # CORNER PIXEL
        if x <= cell_width / 2 and y <= cell_height / 2:
            image[y, x] = lookup_tables[0, 0][image[y, x]]
        elif x <= cell_width / 2 and y >= (len(image) - cell_height / 2):
            image[y, x] = lookup_tables[CELLS_IN_HEIGHT - 1, 0][image[y, x]]
        elif x >= (len(image[0]) - cell_width / 2) and y <= cell_height / 2:
            image[y, x] = lookup_tables[0, CELLS_IN_WIDTH - 1][image[y, x]]
        elif x >= (len(image[0]) - cell_width / 2) and y >= (len(image) - cell_height / 2):
            image[y, x] = lookup_tables[CELLS_IN_HEIGHT - 1, CELLS_IN_WIDTH - 1][image[y, x]]

        # BORDER PIXELS
        elif x <= cell_width / 2:
            x0, x1 = 0, 0
            y0 = int(y / cell_height)
            cell_center = cell_height * y0 + cell_height / 2
            if y < cell_center:
                y1 = y0 - 1
            else:
                y1 = y0 + 1
            k = 1 - np.abs((y - cell_center) / cell_height)
            image[y, x] = k * lookup_tables[y0, x0][image[y, x]] + (1 - k) * lookup_tables[y1, x1][image[y, x]]
        elif y <= cell_height / 2:
            y0, y1 = 0, 0
            x0 = int(x / cell_width)
            cell_center = cell_width * x0 + cell_width / 2
            if x < cell_center:
                x1 = x0 - 1
            else:
                x1 = x0 + 1
            k = 1 - np.abs((x - cell_center) / cell_width)
            image[y, x] = k * lookup_tables[y0, x0][image[y, x]] + (1 - k) * lookup_tables[y1, x1][image[y, x]]
        elif x >= (len(image[0]) - cell_width / 2):
            x0, x1 = CELLS_IN_WIDTH - 1, CELLS_IN_WIDTH - 1
            y0 = int(y / cell_height)
            cell_center = cell_height * y0 + cell_height / 2
            if y < cell_center:
                y1 = y0 - 1
            else:
                y1 = y0 + 1
            k = 1 - np.abs((y - cell_center) / cell_height)
            image[y, x] = k * lookup_tables[y0, x0][image[y, x]] + (1 - k) * lookup_tables[y1, x1][image[y, x]]
        elif y >= (len(image) - cell_height / 2):
            y0, y1 = CELLS_IN_HEIGHT - 1, CELLS_IN_HEIGHT - 1
            x0 = int(x / cell_width)
            cell_center = cell_width * x0 + cell_width / 2
            if x < cell_center:
                x1 = x0 - 1
            else:
                x1 = x0 + 1
            k = 1 - np.abs((x - cell_center) / cell_width)
            image[y, x] = k * lookup_tables[y0, x0][image[y, x]] + (1 - k) * lookup_tables[y1, x1][image[y, x]]

        # OTHERS
        else:
            px, py = int(x / cell_width), int(y / cell_height)
            cx, cy = cell_width * px + cell_width / 2, cell_height * py + cell_height / 2
            p = []
            if x <= cx and y <= cy:
                p.append((py, px))
                p.append((py - 1, px - 1))
                p.append((py, px - 1))
                p.append((py - 1, px))
            elif x >= cx and y <= cy:
                p.append((py, px + 1))
                p.append((py - 1, px))
                p.append((py, px))
                p.append((py - 1, px + 1))
            elif x <= cx and y >= cy:
                p.append((py + 1, px))
                p.append((py, px - 1))
                p.append((py + 1, px - 1))
                p.append((py, px))
            else:
                p.append((py + 1, px + 1))
                p.append((py, px))
                p.append((py + 1, px))
                p.append((py, px + 1))

            cx1, cy1 = cell_width * p[0][1] + cell_width / 2, cell_height * p[0][0] + cell_height / 2
            cx2, cy2 = cell_width * p[1][1] + cell_width / 2, cell_height * p[1][0] + cell_height / 2
            cx3, cy3 = cell_width * p[2][1] + cell_width / 2, cell_height * p[2][0] + cell_height / 2
            cx4, cy4 = cell_width * p[3][1] + cell_width / 2, cell_height * p[3][0] + cell_height / 2
            S = (cx1 - cx2) * (cy1 - cy2)
            k1 = (cx1 - x) * (cy1 - y) / S
            k2 = (x - cx2) * (y - cy2) / S
            k3 = (cx3 - x) * (y - cy3) / S
            k4 = (x - cx4) * (cy4 - y) / S
            image[y, x] = k2 * lookup_tables[p[0][0], p[0][1]][image[y, x]] +\
                          k1 * lookup_tables[p[1][0], p[1][1]][image[y, x]] +\
                          k4 * lookup_tables[p[2][0], p[2][1]][image[y, x]] +\
                          k3 * lookup_tables[p[3][0], p[3][1]][image[y, x]]

plt.imshow(image, cmap='gray')
cv.imwrite('a.png', image)
plt.show()
