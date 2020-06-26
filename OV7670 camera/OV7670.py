import RPi.GPIO as GPIO
import time
import sys

H_ref = 11
V_sync = 13
X_clk = 15
P_clk = 19

d0=12
d1=16
d2=18
d3=22
d4=24
d5=26
d6=32
d7=36

reset=38

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)

GPIO.setup(H_ref,GPIO.IN)
GPIO.setup(V_sync, GPIO.IN)
GPIO.setup(X_clk, GPIO.OUT)
GPIO.setup(P_clk, GPIO.IN)

GPIO.setup(d0, GPIO.IN)
GPIO.setup(d1, GPIO.IN)
GPIO.setup(d2, GPIO.IN)
GPIO.setup(d3, GPIO.IN)
GPIO.setup(d4, GPIO.IN)
GPIO.setup(d5, GPIO.IN)
GPIO.setup(d6, GPIO.IN)
GPIO.setup(d7, GPIO.IN)

GPIO.setup(reset, GPIO.OUT)


##GPIO.output(reset,GPIO.HIGH)
##time.sleep(3/100000000.0)
##GPIO

print("starting!!\n")
GPIO.output(X_clk,GPIO.LOW)
status=0
v_status=0
frame_count=0
pixel_data=[]

while True:
    if len(pixel_data)==640:
        print(frame_count)
        break
    GPIO.output(X_clk,GPIO.HIGH)
    time.sleep(5/100000000.0)
    GPIO.output(X_clk,GPIO.LOW)
    time.sleep(3/100000000.0)
    
    if(not GPIO.input(V_sync) and v_status==1):
        v_status=0

    if(GPIO.input(V_sync) and v_status==0):
        v_status=1
        frame_count=frame_count+1

    if GPIO.input(H_ref) and v_status==1:
        while len(pixel_data) != 640:
            GPIO.output(X_clk,GPIO.HIGH)
            time.sleep(5/100000000.0)
            GPIO.output(X_clk,GPIO.LOW)
            time.sleep(5/100000000.0)
            if (not GPIO.input(P_clk) and status==1):
                status=0
            if (GPIO.input(P_clk) and status==0):
                status=1
                
                d=[]
                d.append(GPIO.input(d0))
                d.append(GPIO.input(d1))
                d.append(GPIO.input(d2))
                d.append(GPIO.input(d3))
                d.append(GPIO.input(d4))
                d.append(GPIO.input(d5))
                d.append(GPIO.input(d6))
                d.append(GPIO.input(d7))
                print(d)
                pixel_data.append(d)



out_string=''            
for each in pixel:
    for i in range(0,8):
        if i==8:
            out_string=out_string+str(bit)+'\n'
        else:    
            out_string=out_string+str(bit)
    


f=open("data.txt","w")
f.write(out_string)
f.close()

print("done!!")

