from camera import Palm_camera
from client import Ethernet_Client
from Run_Encoder import WSQ_Encoder
from time import sleep
import RPi.GPIO as GPIO

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(29,GPIO.OUT)
GPIO.setup(31,GPIO.OUT)
GPIO.setup(40,GPIO.OUT)
GPIO.setup(37,GPIO.OUT)
GPIO.setup(35,GPIO.OUT)
GPIO.setup(36,GPIO.OUT)
GPIO.setup(38,GPIO.OUT)

GPIO.output(40,GPIO.HIGH)
GPIO.output(37,GPIO.HIGH)
GPIO.output(29,GPIO.HIGH)
GPIO.output(31,GPIO.HIGH)

c=Palm_camera()
e=WSQ_Encoder()
t=Ethernet_Client()

GPIO.output(38,GPIO.HIGH)
mean=c.initialize()
print('Initialized !!')
print(mean)
GPIO.output(38,GPIO.LOW)

while True:

    detected=c.Biometric_captured(mean)

    if detected==1:
        GPIO.output(35,GPIO.HIGH)
        encoded=e.run_encoder()
        if encoded ==1:
            send=t.transfer()
        
            if send==1:
                GPIO.output(36,GPIO.HIGH)
                print("Send image for authentication !")
                sleep(10)
    
    GPIO.output(35,GPIO.LOW)
    GPIO.output(36,GPIO.LOW)
