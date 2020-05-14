from camera import Camera
from client import Client


C=Camera(15,13,11,18,22)
def motion():
    while True:
        motion = C.read([0,0,0,0,0,0,1,0])
        if motion[0]==1:
            return C.capture_and_save()
            break


captured_fp = motion()
Client.transfer(captured_fp)

    

