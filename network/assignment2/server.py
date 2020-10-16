import os
import threading
import socket
from datetime import datetime 

def serv_work(client_socket, addr):
    server_name = "2016310932 web server "
    data = client_socket.recv(65535)
    if len(data) < 1 :
        return;
    print(data)
    
    request_data = data.decode().split()
    request_method = request_data[0]
    request_loc = request_data[1]
    request_version = request_data[2]
    if request_method == "GET":
        response_data = "{0} 200 OK\r\nServer: {1}\r\nDate: {2}\r\n".format(
            request_version, server_name,datetime.now().strftime('%a, %d %b %Y %H:%M:%S KST'))
        if request_loc == '/' :
            response_data += "Content-Type: text/html\r\n" + "charset=UTF-8\r\n"
            page = open("./index.html","r")
            page_data = page.read()
            response_data += (page_data +"\r\n\r\n")
            print("sending data:")
            print(response_data)
            client_socket.send(response_data.encode())
        elif request_loc == "/favicon.ico" :
            return;
        elif len(request_loc) > 1:
            print("request loc :"+ request_loc)

            item = open("."+request_loc,"rb")
            item_data = item.read()
            response_data += "Content-Length: %d\r\n" % len(item_data)
            response_data += "Content-Type: image/jpeg\r\n\r\n" 
            # work with 2 newline \r\n\r\n before sending image
            # then send in byte type
            client_socket.sendall(response_data.encode())
            print("sending data:")
            #item_data = str(item_data) +"\r\n\r\n"
            item_data = item_data 
            client_socket.sendall(item_data)
    else:
        response_data = "{0} 405 Method Not Allowed\nServer: {1}\nDate: {2}\n".format(request_version, server_name, 
        datetime.now().strftime('%a, %d %b %Y %H:%M:%S KST'))
    client_socket.close()

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('localhost',10080))
    server_socket.listen(5)
    print('The TCP server is ready to receive')
    while True:
        client_socket, addr = server_socket.accept()
        con_Thread = threading.Thread(target=serv_work,
                        args=(client_socket,addr))
        con_Thread.start()
if __name__ == "__main__":
    main()

#https://noodle-dev.tistory.com/63
#https://kentakang.com/133