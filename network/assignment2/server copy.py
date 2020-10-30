import os
import threading
import socket
from datetime import datetime 
server_name = "2016310932 web server "
response_202 =  "HTTP/1.1 200 OK\r\nServer: {0}\r\n".format(server_name)
response_404 =  "HTTP/1.1 404 NOT FOUND\r\nServer: {0}\r\nContent-Length: 13\r\nContent-Type: text/plain\r\n\r\n404 NOT FOUND".format(
            server_name)
response_403 =  "HTTP/1.1 403 FORBIDDEN\r\nServer: {0}\r\nContent-Length: 13\r\nContent-Type: text/plain\r\n\r\n403 Forbidden".format(
            server_name)
            
# table for ID,PASSWORD
user_table ={"hyun":"1234","aaa":"aaa"}
expire_table ={} # ID / datetime
# cookie for ID
def cookie_setting(ID,PW):
    return "Set-Cookie: id="+ID+"; max-age=30 Secure; \r\n" + "Set-Cookie: password="+PW+";max-age=30 Secure; \r\n"
def cookie_clear():
    text=""
    for ID,PW in user_table.items():
        text += "Set-Cookie: id="+ID+"; max-age=0 Secure; \r\n" + "Set-Cookie: password="+PW+";max-age=0 Secure; \r\n"
    print(text)
    return text
    
def print_info():
    for id,passwd in user_table.items():
        print(id+" "+passwd)
def Login_info(refined_ID,refined_PW):
    for id,passwd in user_table.items():
        #print(id+" "+passwd)
        if (id == refined_ID) and (passwd == refined_PW) :
            return True
    # if not found
    return False
def time_cal(id_compare):
    now = datetime.now()
    for ID in expire_table.keys():
        print(ID)
        if(ID == id_compare):
            diff = (now-expire_table[ID]).seconds
            return str(30 - diff)
    return " "
def serv_work(client_socket, addr):
    #print(client_socket)
    data = client_socket.recv(65535)
    if len(data) < 1 :
        return
    print(data)
    request_data = data.decode().split()
    request_method = request_data[0]
    request_loc = request_data[1]
    request_version = request_data[2]
    if request_method == "GET":
        if request_loc == '/' :
            response_data = response_202 + "Date: {0}\r\n".format(datetime.now().strftime('%a, %d %b %Y %H:%M:%S KST'))
            #response_data += cookie_setting
            if os.path.exists("./index.html"):
                page = open("./index.html","r")
                page_data = page.read()
                response_data += cookie_clear()
                #response_data += "Content-Length: %d\r\n" % len(page_data) # this doesn't work
                response_data += "Content-Type: text/html;charset=UTF-8\r\n\r\n"
                response_data += (page_data +"\r\n\r\n")
                print("sending data:")
                #print(response_data)
                client_socket.sendall(response_data.encode())
            else :
                response_data = response_404 #+ "Date: {2}\r\n".format(datetime.now().strftime('%a, %d %b %Y %H:%M:%S KST'))
                client_socket.sendall(response_data.encode())
        elif request_loc == "/cookie.html":
            request_id = request_data[-2][3:-1]
            request_pw = request_data[-1][9:]
            if Login_info(request_id,request_pw) == False :
                client_socket.sendall(response_403.encode())
            page = open("./cookie.html","r")
            page_data = page.read()
            page_data = page_data.replace("$ID",request_id)
            diff = time_cal(request_id)
            page_data = page_data.replace("$TIME",diff)

            page_len = str(len(page_data))
            response_data = response_202
            response_data += "Content-Length: "+page_len+"\r\nContent-Type: text/html;charset=UTF-8\r\n\r\n"
            client_socket.sendall(response_data.encode())
            client_socket.sendall(page_data.encode())

        #    return;
        elif len(request_loc) > 1:
            # split b/w image & other
            print("request loc :"+ request_loc)
            file_loc = "."+request_loc
            #print(file_loc[-3:])
            # should check privileged to access it
            if os.path.exists(file_loc) :
                request_id = request_data[-2][3:-1]
                request_pw = request_data[-1][9:]
                if Login_info(request_id,request_pw) == False :
                    if "index" not in file_loc:
                        print(file_loc)
                        client_socket.sendall(response_403.encode())
                        return
                response_data = response_202 + "Date: {0}\r\n".format(datetime.now().strftime('%a, %d %b %Y %H:%M:%S KST'))
                item = open(file_loc,"rb")
                item_data = item.read()
                response_data += "Content-Length: {0}\r\n".format(len(item_data))
                if file_loc[-3:] in "jpg/jpeg/png/JPG" :
                    response_data += "Content-Type: image/jpeg\r\n\r\n" 
                elif file_loc[-4:] in "html" :
                    response_data += "Content-Type: text/html;charset=UTF-8\r\n\r\n" 
                elif file_loc[-3:] in "pdf" :
                    response_data += "Content-Type: application/pdf;charset=UTF-8\r\n\r\n" 
                else :
                    response_data += "Content-Type: text/plain;charset=UTF-8\r\n\r\n" 
                client_socket.sendall(response_data.encode())
                print("sending data: {0}".format(file_loc))
                client_socket.sendall(item_data)
            else :
                print("sending data: {0} NOT FOUND".format(file_loc))
                response_data = response_404 #+ "Date: {2}\r\n".format(datetime.now().strftime('%a, %d %b %Y %H:%M:%S KST'))
                #print(response_data)
                client_socket.sendall(response_data.encode())
    elif request_method == "POST":
        #print("here is -1, -2")
        #print(request_data[-1]) # password
        #print(request_data[-2]) # ID
        request_id = request_data[-2][3:]
        request_pw = request_data[-1][9:]
        #print(request_id,request_pw)
        if Login_info(request_id,request_pw) == False :
            client_socket.sendall(response_403.encode())
            return
        print("POST request loc :"+ request_loc)
        file_loc = "."+request_loc
        page = open(file_loc,"r")
        page_data = page.read()
        response_data = response_202 + "Date: {0}\r\n".format(datetime.now().strftime('%a, %d %b %Y %H:%M:%S KST'))
        response_data += cookie_setting(request_id,request_pw)
        expire_table[request_id] = datetime.now()
        #print("setting time: " + str(expire_table[request_id]))
        response_data += "Content-Type: text/html\r\n" + "charset=UTF-8\r\n\r\n"
        response_data += (page_data +"\r\n\r\n")
        print("sending data:")
        print(response_data)
        client_socket.send(response_data.encode())
        
    else: # request is not GET
        response_data = "{0} 405 Method Not Allowed\nServer: {1}\nDate: {2}\n".format(request_version, server_name, 
        datetime.now().strftime('%a, %d %b %Y %H:%M:%S KST'))
    #client_socket.close()

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    #https://stackoverflow.com/questions/17740492/how-i-will-use-setsockopt-and-getsockopt-with-keep-alive-in-linux-c-programming
    server_socket.setsockopt(socket.SOL_SOCKET,socket.SO_KEEPALIVE,1)
    server_socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPIDLE, 1)
    server_socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPINTVL, 3)
    server_socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPCNT, 5)
    server_socket.bind(('',10080))
    server_socket.listen(5)
    print('The TCP server is ready to receive')
    while True:
        client_socket, addr = server_socket.accept()
        con_Thread = threading.Thread(target=serv_work,
                        args=(client_socket,addr))
        con_Thread.start()
        #print(client_socket.fileno())
if __name__ == "__main__":
    #print(Login_info("ID=hwbae","password=1234"))
    main()

#https://noodle-dev.tistory.com/63
#https://kentakang.com/133