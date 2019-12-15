import numpy as np
import matplotlib.pyplot as plt
import cv2 as cv
import scipy.signal as sp


def gaussian_filter(image, size=5):

    filter = np.empty((size, size))
    sigma = 1.4**2
    for x in range(-int(size/2), int(size/2) + 1):
        for y in range(-int(size/2), int(size/2) + 1):
            filter[y + 2, x + 2] = (1 / (2 * np.pi * sigma)) * np.exp(-(x**2 + y**2) / (2 * sigma))
    filter /= np.sum(filter)
    return sp.convolve2d(image, filter, 'same')


def sobel_filter(image):

    filterx = np.array([[-1, 0, 1],
                        [-2, 0, 2],
                        [-1, 0, 1]])
    filtery = np.array([[-1, -2, -1],
                        [ 0,  0,  0],
                        [ 1,  2,  1]])
    imagex = sp.convolve2d(image, filterx, 'same')
    imagey = sp.convolve2d(image, filtery, 'same')

    modules = np.sqrt(imagex**2 + imagey**2)
    directions = np.arctan(imagey / (imagex + 0.0000001))

    return modules, directions


def NMS(modules, directions):
    mask = np.zeros_like(modules, np.bool)
    eps = (np.pi / 4) / 2

    for y in range(len(modules)):
        for x in range(len(modules[0])):
            theta = directions[y, x]
            if -eps <= theta <= eps or -np.pi <= theta <= (-np.pi + eps) or (np.pi - eps) <= theta <= np.pi:
                # horizontal
                x_ind = [i for i in range(x - 1, x + 2) if 0 <= i <= (len(mask[0]) - 1)]
                if x_ind[np.argmax(modules[y, x_ind])] != x:
                    mask[y, x] = True
            elif ((np.pi / 2) - eps) <= theta <= ((np.pi / 2) + eps) or ((-np.pi / 2) - eps) <= theta <= ((-np.pi / 2) + eps):
                # vertical
                y_ind = [i for i in range(y - 1, y + 2) if 0 <= i <= (len(mask) - 1)]
                if y_ind[np.argmax(modules[y_ind, x])] != y:
                    mask[y, x] = True
            elif ((np.pi / 4) - eps) <= theta <= ((np.pi / 4) + eps) or ((-1.5 * np.pi) - eps) <= theta <= ((-1.5 * np.pi) + eps):
                # main diag
                x_ind = [(x + i) for i in range(-1, 2) if
                         0 <= (x + i) <= (len(mask[0]) - 1) and 0 <= (y + i) <= (len(mask) - 1)]
                y_ind = [(y + i) for i in range(-1, 2) if
                         0 <= (x + i) <= (len(mask[0]) - 1) and 0 <= (y + i) <= (len(mask) - 1)]
                if y_ind[np.argmax(modules[(y_ind, x_ind)])] != y:
                    mask[y, x] = True
            else:
                # secondary diag
                x_ind = [(x + i) for i in range(-1, 2) if
                         0 <= (x + i) <= (len(mask[0]) - 1) and 0 <= (y - i) <= (len(mask) - 1)]
                y_ind = [(y - i) for i in range(-1, 2) if
                         0 <= (x + i) <= (len(mask[0]) - 1) and 0 <= (y - i) <= (len(mask) - 1)]
                if y_ind[np.argmax(modules[(y_ind, x_ind)])] != y:
                    mask[y, x] = True

    modules[mask] = 0
    return modules


class RelationshipMap:

    class Tree:

        class Node:
            def __init__(self, id, parent_node):
                self.id = id
                self.parent = parent_node
                self.childs = []

        def __init__(self, id):
            self.root = self.Node(id, None)

        def get_node(self, id):
            nodes = [self.root]
            filler = []
            while len(nodes) != 0:
                node = nodes[0]
                if node in filler:
                    nodes.remove(node)
                    continue
                filler.append(node)
                if node.id == id:
                    return node
                for child in node.childs:
                    nodes.append(child)
                nodes.remove(node)

            return None

    def __init__(self):
        self.trees = []

    def make_set(self, id):
        self.trees.append(self.Tree(id))

    def union(self, from_id, to_id):
        # Find "from" root node
        for tree in self.trees:
            from_node = tree.get_node(from_id)
            if from_node is not None:
                from_node = tree.root
                tree_to_remove = tree
                break
        # Find "to" root node
        for tree in self.trees:
            to_node = tree.get_node(to_id)
            if to_node is not None:
                to_node = tree.root
                break
        # if roots have different id, append "from" to "to"
        if from_node.id != to_node.id:
            to_node.childs.append(from_node)
            from_node.parent = to_node

    def find(self, id):
        for tree in self.trees:
            node = tree.get_node(id)
            if node is not None:
                return tree.root.id

        print('I shouldn\'t be here')
        return None


def canny_edge(modules):
    mid = np.median(modules)
    low_threshold, high_threshold = 15, 30
    modules[modules <= low_threshold] = 0

    # FIRST_PASS
    print('first pass')
    RMap = RelationshipMap()
    set_map = np.zeros_like(modules, dtype=np.int)
    next_set_id = 1
    for y in range(len(modules)):
        for x in range(len(modules[0])):

            if x % 100 == 0 or y % 100 == 0:
                print('first pass', y, x)

            if modules[y, x] != 0:
                up_set_id, left_set_id = 0, 0
                if y != 0:
                    up_set_id = set_map[y-1, x]
                if x != 0:
                    left_set_id = set_map[y, x-1]

                if up_set_id != 0 and left_set_id != 0:
                    set_id = min(up_set_id, left_set_id)
                    RMap.union(max(up_set_id, left_set_id), set_id)
                elif up_set_id != 0:
                    set_id = up_set_id
                elif left_set_id != 0:
                    set_id = left_set_id
                else:
                    set_id = next_set_id
                    RMap.make_set(set_id)
                    next_set_id += 1

                set_map[y, x] = set_id

    #SECOND PASS
    print('second pass')
    for y in range(len(modules)):
        for x in range(len(modules[0])):
            if x % 100 == 0 or y % 100 == 0:
                print('second pass', y, x)
            if set_map[y, x] != 0:
                set_map[y, x] = RMap.find(set_map[y, x])

    for set_id in range(1, next_set_id):
        if len(modules[set_map == set_id]) != 0 and np.max(modules[set_map == set_id]) < high_threshold:
            modules[set_map == set_id] = 0
    modules[set_map != 0] = 255
    return modules


image = np.asarray(cv.imread('emma.jpg', cv.IMREAD_GRAYSCALE), dtype=np.float32)#[::2, ::2]
image = gaussian_filter(image)
modules, directions = sobel_filter(image)
modules = NMS(modules, directions)
edges = canny_edge(np.asarray((modules / np.max(modules)) * 255, np.uint8))

plt.imshow(np.asarray((edges / np.max(edges)) * 255, np.uint8), cmap='gray')
plt.show()
