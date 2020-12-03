import sys,os
import threading,queue
from socket import *
import time
from struct import *
dst_port = 10080
not_ack = 0
ack = 1
copy_lock = threading.Lock()
log_lock = threading.Lock()
packet_q = queue.Queue()
write_q = queue.Queue()
recvAddr =str()
start_time = 0
unique_packet =0
filename = str()
def get_logtime(start_t):
	logging_time = time.time() - start_t
	return round(logging_time,3)
"Use this method to write Packet log"
def writePkt(logFile, procTime, pktNum, event):
    global unique_packet
    unique_packet +=1
    logFile.write('{:1.3f} pkt: {} | {}\n'.format(procTime, pktNum, event))

"Use this method to write ACK log"
def writeAck(logFile, procTime, ackNum, event):
    logFile.write('{:1.3f} ACK: {} | {}\n'.format(procTime, ackNum, event))

"Use this method to write final throughput log"
def writeEnd(logFile, throughput):
    logFile.write('File transfer is finished.\n')
    logFile.write('Throughput : {:.2f} pkts/sec\n'.format(throughput))

    
def packet_process(recv_socket,):
    global recvAddr,start_time,filename
    data_info = dict()
    i = 0
    packet_num=int()
    print("start process")
    expected = 0 # packet num which is expected, if not, send duplicated packet
    
    while True:
        data, addr = recv_socket.recvfrom(1500)
        recvAddr = addr
        # data format : seq_num(4B), windowSize(4B),packet_num(4B) + data(1000B)
        # 1008
        seq_num,windowSize,packet_num = unpack('>iii', data[:12] )
        seq_num = int(seq_num)
        data = data[12:]
        filename, data_left = data.split(b'|',maxsplit=1)
    #    print("hello 2-",seq_num,filename)
        filename = filename.decode()
    #    print(filename,data_left)
        data = data_left#.encode()
            # loggin phase
        with log_lock:
            log_file = open(filename+"_receiving_log.txt","at")
            logger = threading.Thread(target=writePkt,args=(log_file,get_logtime(start_time),seq_num,"received"))
            logger.start()
            logger.join()
            log_file.close()
        if seq_num == 0:
            start_time = time.time()
            #filename = data
            #filename = filename.decode()
            f = open(filename,"wb")
            log_file = open(filename+"_receiving_log.txt","wt")
            f.close()
            log_file.close()
            print("write at "+filename)
            expected = 1
        else :
            #if data_info.get(seq_num) is None:
            #print("store seq_num",seq_num)
            data_info[seq_num] = data
        while data_info.get(expected) is not None:    
        #    f = open(filename,"ab")
        #    f.write(data_info[expected])
        #    f.close()
            #print(data_info[expected])
            #print("write pos ",expected)
            expected +=1
        ack_packet = "ACK:".encode() + pack('>i',expected)
        #if seq_num <=20:
        #    print(ack_packet,recvAddr)
        recv_socket.sendto(ack_packet,recvAddr)

        with log_lock:
            log_file = open(filename+"_receiving_log.txt","at")
            ack_send = threading.Thread(target=writeAck,args=(log_file,get_logtime(start_time),expected,"sent"))
            ack_send.start()
            ack_send.join()
            #writeAck(log_file,get_logtime(start_time),expected,"sent")
            log_file.close()
            #print("end")
        if expected == packet_num+1:
            print("break!")
            break
    
    f = open(filename,"ab")
    for i in range(0,packet_num):
        f.write(data_info[i+1])
    f.close()
    


    end_time = get_logtime(start_time)
    throughput = unique_packet/end_time
    log_file = open(filename+"_receiving_log.txt","at")
    writeEnd(log_file,throughput)
    log_file.close()
    print("process end")
    
if __name__=='__main__':
    recv_socket = socket(AF_INET, SOCK_DGRAM)
    recv_socket.bind(('',dst_port))
    #recv_socket.bind(('',0))
    packet_process(recv_socket)
