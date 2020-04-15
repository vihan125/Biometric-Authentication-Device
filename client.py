import socket
### can use pickel to send arrays ###
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((socket.gethostname(),12301))
n=0
while n<10:
    s.send(bytes("30\n","utf-8"))
    n=n+1

