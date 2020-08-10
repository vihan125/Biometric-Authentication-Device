from abc import ABC, abstractmethod
from picamera import PiCamera
from time import sleep
from PIL import Image


class Abstract_camera(ABC):

    @abstractmethod
    def initialize(self):
        pass

    @abstractmethod
    def Biometric_captured(self):
        pass


class Palm_camera(Abstract_camera):
    
    def __init__ (self):
        self.camera = PiCamera()

    def get_mean(self):
        im = Image.open('image.jpg')
        im_grey = im.convert('LA') # convert to grayscale
        width, height = im.size

        total = 0
        for i in range(0, width):
            for j in range(0, height):
                total += im_grey.getpixel((i,j))[0]

        mean = total / (width * height)
        return mean

    def initialize(self):
        self.camera.start_preview()
        self.camera.resolution = (640, 480)
        total=0
        for i in range(0,3):
            sleep(3)
            self.camera.capture('/home/pi/Biometric_auth/modify/image.jpg')
            total=total+self.get_mean()

        self.camera.stop_preview()
        return (total/3)

    def Biometric_captured(self,mean):
        self.camera.start_preview()
        self.camera.resolution = (640, 480)
        palm_detected=0
        pix_mean=0
        while palm_detected==0:
            sleep(3)
            self.camera.capture('/home/pi/Biometric_auth/modify/image.jpg')
            pix_mean=self.get_mean()
            if (abs(pix_mean-mean)>5):
                palm_detected=1

        self.camera.stop_preview()
        return 1


