import socket

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind((socket.gethostname(),12301))
s.listen(5)

while True:
    msg=''
    clientsocket, address = s.accept()
    n=0
    while n<10:
        msg=msg+clientsocket.recv(10).decode("utf-8")
        n=n+1
    print(msg)
    clientsocket.close()
    #message and the type of data
    

