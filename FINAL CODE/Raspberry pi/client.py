from abc import ABC, abstractmethod
import socket


class Abstract_client(ABC):

    @abstractmethod
    def transfer(self):
        pass
    
class Ethernet_Client(Abstract_client):
    
    def transfer(self):
        
        HEADERSIZE = 10

        s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        s.connect(('192.168.137.1',1265))

        f=open("encode.txt","r")
        data=f.read().split(",")
        f.close()
        if data[len(data)-1]=='':
                data.pop()


        msg=''
        for i in range (0,len(data)):
            msg=msg+str(data[i])+','

        msg=f'{len(msg):<{HEADERSIZE}}' + msg
        s.send(bytes(msg,"utf-8"))
        
        return 1
