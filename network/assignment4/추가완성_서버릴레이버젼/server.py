import sys
from time import sleep
from socket import *
from struct import *
import threading
serverPort = 10080
serverID = "2016310932"
new_regi = 1
broadcast = 2
relay = 3
reqclient_table = 4
req_deregister = 5
keep_alive =6
client_list = dict() # ID : addr
client_pri_ip = dict()
client_time = dict() # ID : recent_access time
def ID_PORT_transform(address):
	# address : tuple type -> (IP,PORT)
	return str(address[0])+":"+str(address[1]) #IP:PORT

def timeout(target_ID):
	global client_list,client_time
	print(target_ID,"is disappear","\t",ID_PORT_transform(client_list[target_ID]))
	try:
		del client_list[target_ID]
		del client_time[target_ID]
		del client_pri_ip[target_ID]
	except KeyError: # already delete
		return
	packet = pack(">h",req_deregister)
	packet += (serverID +"\n\r").encode()
	packet += str(target_ID).encode()
	# broadcast
	tmp_socket = socket(AF_INET, SOCK_DGRAM)
	for ID,address in client_list.items():
		print("to ",address)
		tmp_socket.sendto(packet,address)
	tmp_socket.close()

def server():
	global client_list,client_time
	server_socket = socket(AF_INET,SOCK_DGRAM)
#	server_socket.setsockopt(SOL_SOCKET,SO_REUSEADDR, 1)
	server_socket.bind(("",serverPort))
	print('The UDP server is ready to receive')
	while True:
		data, addr = server_socket.recvfrom(1500)
		instr = unpack(">h",data[:2])
		instr = instr[0]
		data = data[2:].decode()
		clientID,data = data.split("\n\r")
		# instr + ID + data
		#clientPort ,clientID = info.split('|')
		#print("instrction is ",instr,data)
		if instr == keep_alive :
			#print("keep alive request recv :",clientID)
			client_time[clientID].cancel()
			timer = threading.Timer(30,timeout,args=(clientID,))
			timer.start()
			client_time[clientID] = timer
		elif instr == new_regi :
			private_ip = data
			print("private_ip is",private_ip)
			# broadcast action 
			packet = pack(">h",broadcast)
			packet += (serverID +"\n\r").encode()
			packet += (str(clientID)+"|"+str(addr)+"|"+str(private_ip)).encode()
			tmp_socket = socket(AF_INET, SOCK_DGRAM)
			for ID,address in client_list.items():
				tmp_socket.sendto(packet,address)
			tmp_socket.close()
			# register
			if clientID in client_list:
				client_time[clientID].cancel()
			client_list[clientID] = addr
			client_pri_ip[clientID] = private_ip
			timer = threading.Timer(30,timeout,args=(clientID,))
			timer.start()
			client_time[clientID] = timer
			print("======log in ID======")
			print(clientID,"\t",client_list[clientID])
			print("======registered ID======")
			packet = pack(">h",new_regi) 
			packet += (serverID +"\n\r").encode()
			for ID,address in client_list.items():
				print(ID,address)
				packet += (str(ID)+"|"+str(address)+"|"+client_pri_ip[ID]+"\n").encode()
			#print(packet.decode())
			server_socket.sendto(packet,(addr))
		elif instr == relay :
			target_ID, data = data.split("|",1)
			print("To",target_ID,":",data)
			try :
				addr = client_list[target_ID]
			except KeyError:
				print("there is no client :",target_ID)												
			packet = pack(">h",relay) 
			packet += (serverID +"\n\r").encode()
			packet += (clientID +"|" + data).encode()
			server_socket.sendto(packet,addr)
			# relay to target ID
		elif instr == req_deregister :
			target_ID = data
			packet = pack(">h",0) 
			packet += (serverID +"\n\r").encode()
			packet += target_ID.encode()
			server_socket.sendto(packet,client_list[target_ID])
			print(target_ID,"is deregister","\t",ID_PORT_transform(client_list[target_ID]))
			del client_list[target_ID]
			del client_pri_ip[target_ID]
			client_time[target_ID].cancel()
			del client_time[target_ID]
			packet = pack(">h",req_deregister)
			packet += (serverID +"\n\r").encode()
			packet += str(target_ID).encode()
			# broadcast
			tmp_socket = socket(AF_INET, SOCK_DGRAM)
			for ID,address in client_list.items():
				tmp_socket.sendto(packet,address)
			tmp_socket.close()


		"""
		elif instr == req_client_table:
			print("response client_list request")
			packet = pack(">h",req_client_table) 
			packet += (str(serverPort) + "|" + serverID +"\n\r").encode()
			for ID,address in client_list.items():
				print(ID,address)
				packet += (str(ID)+"|"+str(address)+"\n").encode()
			print(packet.decode())
			# ex) c1|('10.0.0.4',60696)
			server_socket.sendto(packet,(addr))
		"""

		#print(addr)
		#print(clientPort,clientID)



"""
Don't touch the code below
"""
if  __name__ == '__main__':
	server()

