import cv2

path = 'CV-2018/Materials/empire.jpg'
img=cv2.imread(path) # BGR
img=cv2.cvtColor(img, cv2.COLOR_BGR2RGB) # convert to RGB

# plt


# save
cv2.imwrite("myimg.jpg", img)