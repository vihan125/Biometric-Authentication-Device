import socket

class Client:
    def transfer(image):
        s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        s.connect((socket.gethostname(),1245))

        for row in image:
            for each in row:
                if image.index(row)==29 and row.index(each)==29:
                    s.send(bytes(str(each),"utf-8"))
                else:
                    s.send(bytes(str(each)+',',"utf-8"))

