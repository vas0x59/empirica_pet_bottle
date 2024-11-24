import socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client_socket.settimeout(1.0)
message = b'test'
addr = ("192.168.4.2", 1978)

# start = time.time()
client_socket.sendto(b"1"*(4*8), addr)