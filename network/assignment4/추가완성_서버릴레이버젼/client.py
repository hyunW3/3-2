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
private_ip = str()
dst = (serverIP,serverPort)
client_list = dict()
client_pri_ip = dict()
exit = 0

def get_ipaddress(): # for private ip
	s= socket(AF_INET,SOCK_DGRAM)
	s.connect(dst)
	r = s.getsockname()[0]
	s.close()
	return r
def get_public_ip(target_ID):
	return client_list[target_ID].split(",")[0][2:-1]
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

def local_recv(local_socket,private_ip):
	global exit 
	while True:
		if exit == 1:
			break
		try:
			data, addr =  local_socket.recvfrom(1500)
		except timeout :
			continue
		instr = unpack(">h",data[:2])
		instr = instr[0]
		data = data[2:].decode()
		ID, data = data.split("\n\r") 
		if instr == chat_data :
			other_client,data = data.split("|",1)
			data = "["+data+"]"
			print("From",other_client,data," via local network")
		else :
			print("wrong instruction :",instr)
	print("local recv end")




def recv_side(send_socket, clientID):
	global exit,client_pri_ip,client_list
	while True:
		data, addr =  send_socket.recvfrom(1500)
		instr = unpack(">h",data[:2])
		instr = instr[0]
		data = data[2:].decode()
		ID, data = data.split("\n\r") 
		#print(instr)
		if instr == register_response:
			const_alive(send_socket,clientID)
			while True:
				if data is str():
					break
				info,data = data.split("\n",1)
				ID,address,other_pri_ip = info.split("|")
				print(ID,address)
				client_list[ID] = address
				client_pri_ip[ID] = other_pri_ip
		elif instr == broadcast_data :
			ID,address,other_pri_ip = data.split("|")
			client_list[ID] = address
			client_pri_ip[ID] = other_pri_ip
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
	print("recv side end")

def client(serverIP, serverPort, clientID):
	global exit,private_ip
	if clientID not in ID_info:
		print("invalid ID")
	#print(clientID,serverIP,serverPort)
	send_socket = socket(AF_INET, SOCK_DGRAM)
	private_ip = get_ipaddress()
	print(private_ip)
	# recv_side
	t1 = threading.Thread(target=recv_side,args=(send_socket,clientID,))
	t1.deamon = True
	t1.start()

	local_socket = socket(AF_INET, SOCK_DGRAM)
	local_socket.settimeout(5)
	local_socket.bind((private_ip,clientPort))
	t2 = threading.Thread(target=local_recv,args=(local_socket,private_ip,))
	t2.deamon = True
	t2.start()
	#threading.Timer(10,const_alive).start()
	#send_socket.bind((serverIP,serverPort))	
	packet = pack(">h",1) # new_regi for server
	packet += (str(clientID) +"\n\r").encode()
	packet += (str(private_ip)).encode()
	send_socket.sendto(packet,dst)

	# after connect to server
	while True:
		data = input("")
		if data is str():
			continue;
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
			if get_public_ip(target_ID) !=  get_public_ip(clientID):
				#print(get_public_ip(target_ID),get_public_ip(clientID))
				send_socket.sendto(packet,dst)
			else :
				# target IP in same NAT
				addr = (client_pri_ip[target_ID],clientPort)
				print("send locally",addr)
				send_socket.sendto(packet,addr)

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
	print("client close.. waiting for timer...")
	t1.join()
	t2.join()
	local_socket.close()
	send_socket.close()

	return


if  __name__ == '__main__':
	clientID = input("Enter ID: ")
	client(serverIP, serverPort, clientID)
#The client program must send a registration request to a server with UDP with port number 10081.


