import sys,os,stat
import threading
from socket import *
import time
from struct import *
dst_port = 10080
unit_size = 1400
unique_packet =0
not_ack = 0
start_time = 0
#end_time = 0
recvAddr = 0
ack = 1
windowSize = 0
total_RTT = 0
avg_RTT = 1
pkt_t_info = dict()
ack_t_info = dict()
pos_lock = threading.Lock()
log_lock = threading.Lock()

pos = 0 # for window size 
	# 	| ---------------- |	queue size
	#     |-----| 				window size
	#	  | 				pos
def cal_avg_RTT(seq_num):
	# in log_lock
	global avg_RTT,total_RTT,pkt_t_info,ack_t_info
	try :
		a = pkt_t_info[seq_num-1]
		b = ack_t_info[seq_num]
	except KeyError:
		return 1
	if (a is not None) and (b is not None):
		total_RTT += (b-a)
	else :
		print(a,b)
	avg_RTT = round(total_RTT/seq_num,3)
	if avg_RTT < 0.1:
		avg_RTT = 0.1
def get_logtime(start_t):
	logging_time = time.time() - start_t
	return round(logging_time,3)
"Use this method to write Packet log"
def writePkt(logFile, procTime, pktNum, event):
	global unique_packet
	#with count_lock:
	unique_packet +=1
	logFile.write('{:1.3f} pkt: {} | {}\n'.format(procTime, pktNum, event))

"Use this method to write ACK log"
def writeAck(logFile, procTime, ackNum, event):
	#global unique_packet
	#with count_lock:
	#	unique_packet +=1
	logFile.write('{:1.3f} ACK: {} | {}\n'.format(procTime, ackNum, event))

"Use this method to write final throughput log"
def writeEnd(logFile, throughput, avgRTT):
    logFile.write('File transfer is finished.\n')
    logFile.write('Throughput : {:.2f} pkts/sec\n'.format(throughput))
    logFile.write('Average RTT : {:.1f} ms\n'.format(avgRTT))
def timer(send_socket,packet_num,srcFilename,data_info,dstFilename):
	global avg_RTT,pkt_t_info,ack_t_info,start_time,windowSize
	global recvAddr
	pos_lock.acquire()
	local_pos = int()
	while pos <= packet_num :
		send_time = int()
		try :
			if pos_lock.locked():
				pos_lock.release()
			send_time = pkt_t_info[pos]
			local_pos = pos
			#print(send_time,local_pos)
		except KeyError:
			if not pos_lock.locked():
				pos_lock.acquire()
			continue
		#print(pos,get_logtime(start_time),send_time,avg_RTT*2)
		if(get_logtime(start_time) - send_time > avg_RTT*2):
			# retransmit
			#print("pos :",local_pos,"timeout!")
			packet = pack('>iii', local_pos,  windowSize,packet_num )
		# H 2byte, L 4 bytes -> 8
			packet += dstFilename.encode() 
			packet += "|".encode()
			#print("timeout1",local_pos,data_info[local_pos])
			packet += data_info[local_pos]
			#print("timeout2",send_socket,packet,recvAddr,dst_port)
			send_socket.sendto(packet,(recvAddr,dst_port))
			resend_time = get_logtime(start_time)
			with log_lock:
				log_file = open(srcFilename+"_sending_log.txt","at")
				#log_file.write('{:1.3f} pkt: {} | {}\n'.format(resend_time,local_pos,"timeout" ))
				log_file.write('{:1.3f} pkt: {} | {}\n'.format(resend_time,local_pos,"timeout since "+str(send_time)+"(timeout value "+str(round(avg_RTT,1))+")"))
				pkt_t_info[local_pos] = resend_time
				logger = threading.Thread(target=writePkt,args=(log_file,resend_time,local_pos,"retransmitted"))
				logger.start()
				logger.join()
				log_file.close()
		time.sleep(avg_RTT)
		pos_lock.acquire()
		#if local_pos == pos :
		
		#	pos_lock.release()
		#	break


def ack_receiver(packet_num,srcFilename,data_ack_info,data_info):
	global pos,start_time,avg_RTT # first packet number for window
	pos = 1
	enter = 0
	pos_lock.acquire()
	while pos <= packet_num+1 :
		pos_lock.release()
		data, addr = send_socket.recvfrom(8)
        # data format : ack_packet = "ACK:".encode() + pack('>i',expected)
		#recvAddr = addr
		seq_num = unpack('>i',data[4:])
		seq_num = int(str(seq_num[0]))
		data_ack_info[seq_num] += ack
		if seq_num >= packet_num:
			print(seq_num,data)
		with log_lock:
			log_file = open(srcFilename+"_sending_log.txt","at")
			recv_time = get_logtime(start_time)
			ack_t_info[seq_num] = recv_time
			cal_avg_RTT(seq_num)
			logger = threading.Thread(target=writeAck,args=(log_file,recv_time,seq_num,"received"))
			logger.start()
			logger.join()
			log_file.close()
		if(seq_num == packet_num+1):
			print("exit ack recevier ")
			break
		#if pos >= 846  :
		#	print("at seq is ",seq_num,", pos:",pos,packet_num)
		#	if pos == packet_num+1:
		#		print(seq_num)
		if data_ack_info[seq_num] >= ack + 3*ack:
			packet = pack('>iii', seq_num,  windowSize,packet_num )
			packet += dstFilename.encode() 
			packet += "|".encode()
			packet += data_info[seq_num]
			send_socket.sendto(packet,(recvAddr,dst_port))
			with log_lock:
				log_file = open(srcFilename+"_sending_log.txt","at")
				resend_time = get_logtime(start_time)
				pkt_t_info[seq_num] = resend_time
				logger = threading.Thread(target=writePkt,args=(log_file,resend_time,seq_num,"3 duplicated ACKs"))
				logger.start()
				logger.join()
				logger = threading.Thread(target=writePkt,args=(log_file,resend_time,seq_num,"retransmitted"))
				logger.start()
				logger.join()
				log_file.close()
		pos_lock.acquire()
		#print("ack seq number:",seq_num,"pos is",pos)
		if pos < seq_num:
			pos = seq_num
		#while (data_ack_info[pos] != not_ack): # go to next window start
		#	pos +=1	
	print("ack receiver exit")
	if pos_lock.Locked():
		pos_lock.release()
		#if seq_num < 5:
def pkt_sender(packet_num,srcFilename,data_info,send_socket):
	global start_time,pos,windowSize
	i = 0
	print("packet_num:",packet_num)	
	while True:
		#print(i,pos,windowSize)
		pos_lock.acquire()
		while (i-pos < windowSize): #(pos< packet_num): #   or
		#(i <= packet_num) and
			pos_lock.release()
			packet = pack('>iii', i,  windowSize,packet_num ) # i 4bytes -> 12
		# H 2byte, L 4 bytes -> 8
			packet += dstFilename.encode() 
			packet += "|".encode()
			#print(packet)
			if i == 0 :
				start_time = time.time()
			else :
				packet += data_info[i-1]
			with log_lock :
				log_file = open(srcFilename+"_sending_log.txt","at")
				send_time = get_logtime(start_time)
				pkt_t_info[i] = send_time
				logger = threading.Thread(target=writePkt,args=(log_file,send_time,i,"sent"))
				logger.start()
				logger.join()
				log_file.close()
			if i >= packet_num:
				print(recvAddr)
			send_socket.sendto(packet,(recvAddr,dst_port))
			#print(i)
			i +=1
			if pos == packet_num+1:
				print("sender exit!",pos,i)
					#pos_lock.release()
				break
			pos_lock.acquire()
		if(pos == packet_num+1):
			print("sender exit!",pos)
			pos_lock.release()
				#pos_lock.release()
			break
		else :
			#print("hello",pos)
			pos_lock.release()
			time.sleep(1/1000000)
	print("packet sender end")
def fileSender(send_socket):
	global unique_packet,total_RTT,avg_RTT
	global pkt_t_info,ack_t_info
	print('sender program starts...') 
	
	print(recvAddr)
	print("window size is ",windowSize)
	print(srcFilename)
	print(dstFilename)
    ########################## 
    #Write your Code here
	data_info = []
	data_ack_info = []
	#data read section
	f = open(srcFilename,'rb')
	log_file = open(srcFilename+"_sending_log.txt","wt")
	os.system('chown mininet '+srcFilename+'_sending_log.txt')
	#os.chmod(srcFilename+"_sending_log.txt", stat.S_IWRITE)
	os.chmod(srcFilename+"_sending_log.txt",436)
	log_file.close()
	f.seek(0,2)
	fsize = f.tell()
	f.seek(0)
	i = 0
	while i*unit_size < fsize:
		data = f.read(unit_size)
		data_info.append(data)
		data_ack_info.append(not_ack)
		#if i <=2:
		#print(str(i) + " "  + str(data_ack_info[i]))
		i +=1
	f.close()
	packet_num = i
	data_ack_info.append(not_ack) # first packet for filename
	data_ack_info.append(not_ack) # last packet for finish
	print("number of packet:",packet_num)
	#print(data_info[0])
	#data send section
	
	#check ack
	ack_recv = threading.Thread(target=ack_receiver,args=(packet_num,srcFilename,data_ack_info,data_info))
	ack_recv.start()
	pkt_send = threading.Thread(target=pkt_sender,args=(packet_num,srcFilename,data_info,send_socket))
	pkt_send.start()
	timer_t = threading.Thread(target=timer, args=(send_socket,packet_num,srcFilename,data_info,dstFilename))
	timer_t.start()

	timer_t.join()
	pkt_send.join()
	ack_recv.join()
	end_time = get_logtime(start_time)
  	#wrote end
	print("file sender end")
	# def writeEnd(logFile, throughput, avgRTT):
	throughput = unique_packet/end_time
	avg_RTT = total_RTT/(packet_num+1)
	print("hello",unique_packet,end_time)
	log_file = open(srcFilename+"_sending_log.txt","at")
	writeEnd(log_file,throughput, avg_RTT)
	log_file.close()
	print('File transfer is finished.\n')

	return
	
	
	##########################


if __name__=='__main__':
	recvAddr = sys.argv[1]  #receiver IP address
	windowSize = int(sys.argv[2])   #window size
	srcFilename = sys.argv[3]   #source file name
	dstFilename = sys.argv[4]   #result file name
	send_socket = socket(AF_INET,SOCK_DGRAM)
	send_socket.setsockopt(SOL_SOCKET,SO_REUSEADDR, 1)
	send_socket.bind(("",dst_port))	
	fileSender(send_socket)
