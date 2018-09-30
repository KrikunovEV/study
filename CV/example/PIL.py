from PIL import Image

path = 'CV-2018/Materials/empire.jpg'
img = Image.open(path) # is not numpy
img = np.array(img) # is