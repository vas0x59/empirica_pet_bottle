import socket
import struct
import time
serverSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Assign IP address and port number to socket
serverSocket.bind(("", 8888))
# socket.socket.

f = open("data_recv", "wb")

t0 = 0
t0_esp = 0
while True:
    # rand = random.randint(0, 10)
    b, address = serverSocket.recvfrom(1024)
    if t0 == 0:
        t0 = time.time()
    f.write(b)
    data = struct.unpack("<Lffffffffff", b)
    t = data[0] / 1000
    print(data)
    if t0_esp == 0:
        t0_esp = t
    print(t - t0_esp, time.time()-t0)
# f.close()