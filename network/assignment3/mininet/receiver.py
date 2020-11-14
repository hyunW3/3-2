import sys,os
import threading
from socket import *
import time
port = 10080
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
def writeEnd(logFile, throughput):
    logFile.write('File transfer is finished.')
    logFile.write('Throughput : {:.2f} pkts/sec'.format(throughput))


def fileReceiver():
    print('receiver program starts...')

    #########################
    #Write your Code here
    while True:
        data, addr = recv_socket.recvfrom(1000)
        print(addr,data)
    #########################



if __name__=='__main__':
    recv_socket = socket(AF_INET, SOCK_DGRAM)
    recv_socket.bind(('',port))
    #recv_socket.bind(('',0))
    
    fileReceiver()
