import os
import threading
import socket
import datetime 

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_socket.bind(('localhost',10080))
server_socket.listen(5)
print('The TCP server is ready to receive')
while True:
    client_socket, addr = server_socket.accept()
    data = client_socket.recv(65535)
    request_data = data.decode().split()
    request_method = request_data[0]
    request_version = request_data[2]
    server_name = "web server for network"
    if request_method == "GET":
        response_data = "{0} 200 OK\nServer: {1}\nDate: {2}\n".format(request_version, server_name, 
        datetime.now().strftime('%a, %d %b %Y %H:%M:%S KST'))
    else:
        response_data = "{0} 405 Method Not Allowed\nServer: {1}\nDate: {2}\n".format(request_version, server_name, 
        datetime.now().strftime('%a, %d %b %Y %H:%M:%S KST'))
print(data)

#https://noodle-dev.tistory.com/63
#https://kentakang.com/133