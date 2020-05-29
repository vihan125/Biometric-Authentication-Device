from PIL import Image
from numpy import *


im = array(Image.open('fp1.png').convert('L'))
print (im.shape, im.dtype)

data=''
for i in range(0,30):
    for j in range(0,30):
        data=data+str(im[i][j])+','

print(data)
        
