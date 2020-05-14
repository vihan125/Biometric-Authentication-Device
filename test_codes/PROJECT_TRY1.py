import RPi.GPIO as GPIO
import time
import sys

clk = 15
miso = 13
mosi = 11
cs = 18
reset = 22


GPIO.setwarnings(False)

GPIO.setmode(GPIO.BOARD)
GPIO.setup(reset, GPIO.OUT)
GPIO.setup(clk, GPIO.OUT)
GPIO.setup(miso, GPIO.IN)
GPIO.setup(mosi, GPIO.OUT)
GPIO.setup(cs, GPIO.OUT)

GPIO.output(reset, GPIO.HIGH)
time.sleep(1)
GPIO.output(reset, GPIO.LOW)
time.sleep(5)

GPIO.output(cs, GPIO.HIGH)

def read (address):
    
    GPIO.output(cs, GPIO.LOW)
    GPIO.output(clk, GPIO.LOW)
    for i in range(0,8):
        if i==7:
            if address[i]==1:
                GPIO.output(mosi, GPIO.HIGH)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.HIGH)

       
            else:
                GPIO.output(mosi, GPIO.LOW)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.HIGH)

       
        else:
            if address[i]==1:
                GPIO.output(mosi, GPIO.HIGH)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.HIGH)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.LOW)
       
            else:
                GPIO.output(mosi, GPIO.LOW)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.HIGH)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.LOW)

    data=[]
    time.sleep(0.005)

    for j in range(0,8):
        GPIO.output(clk, GPIO.LOW)
        time.sleep(3/1000000.0)
        if GPIO.input(miso):
            data.append(1)
        else:
            data.append(0)
        GPIO.output(clk, GPIO.HIGH)
        time.sleep(3/1000000.0)

    GPIO.output(cs, GPIO.HIGH)
    return data

def write (address,data):
    
    message=address+data
    
    GPIO.output(cs, GPIO.LOW)
    GPIO.output(clk, GPIO.LOW)
    for i in range(0,16):
            if message[i]==1:
                GPIO.output(mosi, GPIO.HIGH)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.HIGH)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.LOW)
       
            else:
                GPIO.output(mosi, GPIO.LOW)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.HIGH)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.LOW)
                
    GPIO.output(cs, GPIO.HIGH)

def read_continous():
    data=[]
    for j in range(0,8):
        GPIO.output(clk, GPIO.LOW)
        time.sleep(3/1000000.0)
        if GPIO.input(miso):
            data.append(1)
        else:
            data.append(0)
        GPIO.output(clk, GPIO.HIGH)
        time.sleep(3/1000000.0)

    return data

def capture_frame():
    write([1,0,0,1,0,0,1,1],[1,0,0,0,0,0,1,1])
    time.sleep(1)
    pixel_burst=[0,1,0,0,0,0,0,0]
    GPIO.output(cs, GPIO.LOW)
    GPIO.output(clk, GPIO.LOW)
    for i in range(0,8):
        if i==7:
            if pixel_burst[i]==1:
                GPIO.output(mosi, GPIO.HIGH)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.HIGH)

       
            else:
                GPIO.output(mosi, GPIO.LOW)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.HIGH)

       
        else:
            if pixel_burst[i]==1:
                GPIO.output(mosi, GPIO.HIGH)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.HIGH)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.LOW)
       
            else:
                GPIO.output(mosi, GPIO.LOW)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.HIGH)
                time.sleep(3/1000000.0)
                GPIO.output(clk, GPIO.LOW)
                
    time.sleep(1)

    no_pixels=0
    pixels=[]
    while no_pixels < 900:
        pixels.append(read_continous())
        no_pixels = no_pixels+1

    return pixels
        

data= capture_frame()
pix_values=[]
image=[]
for each in data:
    new = each[2:]
    value=0
    for i in range(0,6):
        value=value+(2**i)*new.pop()
    value=(value/63)*255   
    pix_values.append(int(value))
    if len(pix_values)==30:
        pix_values.reverse()
        image.append(pix_values)
        pix_values=[]
    else:
        continue
        
image.reverse()
print(image)
