import json
import numpy as np

path = "C:/Users/Evgenii/Desktop/dataset/100k/data/"
f = open(path + "images/images.json", 'r')
images = json.load(f)[:10000]
f.close()

f = open("10k/images.json", 'w')
json.dump(images,f)
f.close()




f = open(path + "labels/labels.json", 'r')
labels = json.load(f)[:10000]
f.close()

f = open("10k/labels.json", 'w')
json.dump(labels,f)
f.close()



f = open(path + "coords/coords.json", 'r')
coords = json.load(f)[:10000]
f.close()

f = open("10k/coords.json", 'w')
json.dump(coords,f)
f.close()