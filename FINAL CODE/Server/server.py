import socket
import time
import matplotlib.pyplot as plt
import numpy as np
from creating_image import CImage
from Run_decoder import Decode_Image




HEADERSIZE = 10
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind((socket.gethostname(),1265))
s.listen(5)

full_msg=''
new_msg = True

while True:
    done=0
    clientsocket, address = s.accept()
    while done==0:
        msg=clientsocket.recv(16)
        if new_msg:
            print("new client!")
            msglen = int(msg[:HEADERSIZE])
            new_msg = False

        full_msg = full_msg+msg.decode("utf-8")
        if len(full_msg)-HEADERSIZE == msglen:
            new_msg = True
            f=open("decode.txt","w")
            f.write(full_msg[HEADERSIZE:])
            f.close()
            status=Decode_Image.decode_data()
            print("image data decoded")
            if status==1:
                CImage.create_image()
            else:
                print("problems in decoding !")
            full_msg=''
            done=1
            
    print("closing socket !")
    clientsocket.close()
