import socket
import time
import matplotlib.pyplot as plt
import numpy as np

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind((socket.gethostname(),1245))
s.listen(5)

data=[]
msg=''

while True:
    clientsocket, address = s.accept()

    while len(data)!= 900:
        msg=msg+clientsocket.recv(100).decode("utf-8")
        data=msg.split(',')
        if data[len(data)-1]=='':
            data.pop()

    image=[]
    temp=[]
    for each in data:
        if len(temp)==29:
            temp.append(int(each))
            image.append(temp)
            temp=[]
        else:
            temp.append(int(each))
    print(image)
    d=np.array(image)
    plt.gray()
    plt.show(d)
    plt.imsave("transfer3.jpg",d)
    clientsocket.close()
