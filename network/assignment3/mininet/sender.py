import sys,os
import threading
from socket import *
import time
dst_port = 10080
unit_size = 1000
not_ack = 0
ack = 1
"Use this method to write Packet log"
def writePkt(logFile, procTime, pktNum, event):
    logFile.write('{:1.3f} pkt: {} | {}'.format(procTime, pktNum, event))

"Use this method to write ACK log"
def writeAck(logFile, procTime, ackNum, event):
    procTime = time.time() - startTime
    logFile.write('{:1.3f} ACK: {} | {}'.format(procTime, ackNum, event))

"Use this method to write final throughput log"
def writeEnd(logFile, throughput, avgRTT):
    logFile.write('File transfer is finished.')
    logFile.write('Throughput : {:.2f} pkts/sec'.format(throughput))
    logFile.write('Average RTT : {:.1f} ms'.format(avgRTT))

def sending(packet_num,data_info):
	#data send section
	send_socket.sendto(dstFilename.encode('utf-8'),(recvAddr,dst_port))
	i = 0
	pos = 0 # for window size
	while (i < packet_num): # (pos<= packet_num) or
		#print(str(i)+str(data_info[i]))
		send_socket.sendto(data_info[i],(recvAddr,dst_port))
		i +=1
	#	while(data_ack_info[pos] == not_ack):
	#		pos +=1		
	return

def fileSender():
	print('sender program starts...') 
	
	print(recvAddr)
	print(windowSize)
	print(srcFilename)
	print(dstFilename)
    ########################## 
    #Write your Code here
	data_info = []
	data_ack_info = []
	#data read section
	f = open(srcFilename,'rb')
	log_file = open(srcFilename+"_sending_log.txt","w")
	f.seek(0,2)
	fsize = f.tell()
	f.seek(0)
	i = 0 
	while i*unit_size < fsize:
		data = f.read(unit_size)
		data_info.append(data)
		data_ack_info.append(not_ack)
	#	print(str(i) + " "  + str(data_ack_info[i]))
		i +=1
	f.close()
	packet_num = i
	#print(packet_num)
	#print(data_info[0])
	#data send section
	sender = threading.Thread(target=sending,args=(packet_num,data_info,))
	
	sender.start()		
	#check ack

  	#wrote end


	
	
	##########################


if __name__=='__main__':
	recvAddr = sys.argv[1]  #receiver IP address
	windowSize = int(sys.argv[2])   #window size
	srcFilename = sys.argv[3]   #source file name
	dstFilename = sys.argv[4]   #result file name
	send_socket = socket(AF_INET,SOCK_DGRAM)
	send_socket.setsockopt(SOL_SOCKET,SO_REUSEADDR, 1)
	send_socket.bind(("",dst_port))	
	fileSender()
