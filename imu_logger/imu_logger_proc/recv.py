import socket
import struct
serverSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Assign IP address and port number to socket
serverSocket.bind(("", 8888))
# socket.socket.

f = open("data_recv", "wb")

while True:
    # rand = random.randint(0, 10)
    b, address = serverSocket.recvfrom(1024)
    f.write(b)
    print(b)
    print(struct.unpack("<Lfffffff", b))
# f.close()