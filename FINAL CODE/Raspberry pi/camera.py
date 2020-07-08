from picamera import PiCamera
from time import sleep
from PIL import Image

    
camera = PiCamera()
    
def get_mean():
    im = Image.open('image.jpg')
    im_grey = im.convert('LA') # convert to grayscale
    width, height = im.size

    total = 0
    for i in range(0, width):
        for j in range(0, height):
            total += im_grey.getpixel((i,j))[0]

    mean = total / (width * height)
    return mean

def initialize():
    camera.start_preview()
    total=0
    for i in range(0,3):
        sleep(3)
        camera.resolution = (640, 480)
        camera.capture('/home/pi/Biometric_auth/final/image.jpg')
        total=total+get_mean()

    camera.stop_preview()
    return (total/3)

def palm_captured(mean):
    camera.start_preview()
    palm_detected=0
    pix_mean=0

    while palm_detected==0:
        sleep(3)
        camera.resolution = (640, 480)
        camera.capture('/home/pi/Biometric_auth/final/image.jpg')
        pix_mean=get_mean()
        if (abs(pix_mean-mean)>10):
            palm_detected=1

    camera.stop_preview()
    return 1


