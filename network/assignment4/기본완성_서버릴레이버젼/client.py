import sys,os
from socket import *
from struct import *
import threading

serverIP = '10.0.0.3'
serverPort = 10080
clientPort = 10081
register_response = 1
broadcast_data = 2
chat_data = 3
broadcast_deregi = 5
im_alive =6
ID_info =["c1","c2","c3","c4","c5","c6"]
my_addr = ()
dst = (serverIP,serverPort)
client_list = dict()
exit = 0
def ID_PORT_transform(address):
	# address : tuple type -> (IP,PORT)
	IP,PORT = address.split(",")
	IP = IP[2:-1]
	PORT = PORT[1:-1]
	addr = IP+":"+PORT
	return addr #IP:PORT
def const_alive(send_socket, clientID):
	global exit
	if(exit == 1):
		print("stop sending alive message")
		return
		#timer.cancel()
	packet = pack(">h",im_alive)
	packet += (str(clientID) +"\n\r").encode()
	send_socket.sendto(packet,dst)
	#print("send keep alive func")
	timer = threading.Timer(10,const_alive,args=[send_socket,clientID])
	timer.start()


def recv_side(send_socket, clientID):
	global exit
	while True:
		data, addr =  send_socket.recvfrom(1500)
		instr = unpack(">h",data[:2])
		instr = instr[0]
		data = data[2:].decode()
		ID, data = data.split("\n\r") 
		if instr == register_response:
			const_alive(send_socket,clientID)
			while True:
				if data is str():
					break
				info,data = data.split("\n",1)
				ID,address = info.split("|")
				print(ID,address)
				client_list[ID] = address
		elif instr == broadcast_data :
			ID,address = data.split("|")
			client_list[ID] = address
			print(ID,ID_PORT_transform(address),"is added")
		elif instr == chat_data:
			other_client,data = data.split("|",1)
			data = "["+data+"]"
			if other_client == clientID :
				data += "is return : NO CLIENT CONNECTED"
			print("From",other_client,data)
		elif instr == broadcast_deregi :
			print(data,client_list[data],"is deleted")

			del client_list[data]
		elif instr == 0: # exit
			exit = 1
			break
		else :
			print("wrong instr",instr)

def client(serverIP, serverPort, clientID):
	global exit
	if clientID not in ID_info:
		print("invalid ID")
	#print(clientID,serverIP,serverPort)
	send_socket = socket(AF_INET, SOCK_DGRAM)
	# recv_side
	t1 = threading.Thread(target=recv_side,args=(send_socket,clientID,))
	t1.deamon = True
	t1.start()
	#threading.Timer(10,const_alive).start()
	#send_socket.bind((serverIP,serverPort))	
	packet = pack(">h",1) # new_regi for server
	packet += (str(clientID) +"\n\r").encode()
	send_socket.sendto(packet,dst)
	#print("======registered ID info======")
#	for a,b in client_list.items():
#		print(a,"is",b)
	# after connect to server
	while True:
		data = input("")
		if data[0] is not "@":
			print("include command @ !")
			continue
		if data == "@show_list" :
			print("======registered ID info======")
			for ID,address in client_list.items():
				print(ID,ID_PORT_transform(address))
		elif data.find('@chat',0,5) == 0:
			instr,target_ID,data = data.split(" ",2)
			#print(target_ID, data) 
			packet = pack(">h",3) # relay chat via server
			packet += (str(clientID) +"\n\r").encode()
			packet += (str(target_ID)+ "|" +str(data)).encode() # data
			send_socket.sendto(packet,dst)
		elif data == "@exit" :
			# request deregister 
			packet = pack(">h",5) # req_deregister in server
			packet += (str(clientID) +"\n\r").encode()
			packet += str(clientID).encode() # data
			send_socket.sendto(packet,dst)
			exit = 1
			break
		else :
			print("invalid input")
		#print(data)
	t1.join()
	print("client close.. waiting for timer...")
	return


if  __name__ == '__main__':
    clientID = input("Enter ID: ")
    client(serverIP, serverPort, clientID)
#The client program must send a registration request to a server with UDP with port number 10081.


