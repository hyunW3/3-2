from socket import *
import threading
import time
from struct import *
import random
import queue


def packet_capture(clientSocket, probability, order, buffer_size, receive_queue, des_name):
    global oldest, remain_buffer_size, start
    remain_buffer_size = buffer_size
    oldest = 0
    receive_oldest = 0
    start = 0
    while True:
            data = clientSocket.recv(1400)
            if oldest > receive_oldest :
                receive_oldest = oldest
            
            if random.random() < probability :
                seq_num, ack_num = unpack('>LL', data[4:12])
                seq_num = int(seq_num)
                if start == 0:
                    start = time.time()
                ta = threading.Thread(target = log_file_write, args = (des_name, seq_num, time.time() - start, 0, ))
                ta.start()
                ta.join()
                ta = threading.Thread(target = log_file_write, args = (des_name, seq_num, time.time() - start, 2, ))
                ta.start()
                ta.join()
                continue
            
            if remain_buffer_size >= 1400 :
                remain_buffer_size -= 1400
                seq_num, ack_num = unpack('>LL', data[4:12])
                seq_num = int(seq_num)
                receive_queue.put(data)
                if start == 0:
                    start = time.time()
                tb = threading.Thread(target = log_file_write, args = (des_name, seq_num, time.time() - start, 0, ))
                tb.start()
                tb.join()
                
            if oldest == order:
                break
            else:
                continue
            
        


def packet_process(clientSocket, clientAddress, des_name, order, window_size, receive_queue):
    global oldest, remain_buffer_size, start
    temp = dict()
    i = 0
    oldest = 0
    cumul_ack = 0
    des_file = open(des_name, 'wb')
    while True:
            data = receive_queue.get()
            remain_buffer_size += 1400
            seq_num, ack_num = unpack('>LL', data[4:12])
            seq_num = int(seq_num)
            ack_num = int(ack_num)
            temp[seq_num] = data
            if seq_num > oldest:
                if oldest != 0:
                    ack_data = pack('>HHLLHHL', 10080, 0, 0, oldest- 1, 0, window_size, 0)
                    clientSocket.sendto(ack_data, clientAddress)
                tc = threading.Thread(target = log_file_write, args = (des_name, oldest, time.time() - start, 1, ))
                tc.start()
                tc.join()

            elif seq_num == oldest:
                des_file.write(data[20:])
                ack_data = pack('>HHLLHHL', 10080, 0, 0, seq_num, 0, window_size, 0)
                clientSocket.sendto(ack_data, clientAddress)
                td = threading.Thread(target = log_file_write, args = (des_name, oldest, time.time() - start, 1, ))
                td.start()
                td.join()
                oldest = oldest + 1

                while  temp.get(oldest):
                    data = temp[oldest]
                    oldest = oldest + 1
                    des_file.write(data[20:])
                    cumul_ack = 1

                if cumul_ack == 1:
                    cumul_ack = 0
                    ack_data = pack('>HHLLHHL', 10080, 0, 0, oldest - 1, 0, window_size, 0)
                    clientSocket.sendto(ack_data, clientAddress)
            
            if oldest == order :
                tk = threading.Thread(target = log_file_write, args = (des_name, oldest, time.time() - start, 3, ))
                tk.start()
                tk.join()
                des_file.close()
                break


def log_file_write(des_name, num, time, sent):
    with thread_lock:

        while True:
            try:
                log_file = open(des_name + '_receving_log.txt','a')
                break
            except PermissionError :
                continue
            
        delta = "%0.3f"% float(time)
        
        if sent == 1 :
            log_write = str(str(delta) + " ACK: " + str(num) +" | sent\n")
        elif sent == 0:
            log_write = str(str(delta) + " pkt: " + str(num) +" | received\n")
        elif sent == 2:
            log_write = str(str(delta) + " pkt: " + str(num) +" | dropped\n")
        else :
            goodput = "%0.3f"% (num/float(time))
            log_write = "\n" + "File transfer is finished." + "\n" + "Throughput: " + str(goodput) + " pkts / sec"
        
        if not log_write == 'ed' :
            log_file.write(log_write)
        log_file.close()


if __name__ == '__main__':
        probability = 10.0
        while probability < 0.0 or probability > 1.0:
            probability = float(input('Packet loss probability: '))
        buffer_size = int(input('Socket recv buffer size: '))
        if buffer_size <= 1000000:
            buffer_size = 1000000
        thread_lock = threading.Lock()
        print('socket recv buffer size updated: ' + str(buffer_size))
        print('')
        print('receiver program starts...')
    #while True:
        clientSocket = socket(AF_INET, SOCK_DGRAM)
        clientSocket.bind(("127.0.0.1",10080))
        data, clientAddress = clientSocket.recvfrom(buffer_size)
        while random.random() < probability:
            ack_data = pack('>HHLLHHL', 10080, 0, 0, -1, 0, window_size, 0)
            clientSocket.sendto(ack_data, clientAddress)
            data, clientAddress = clientSocket.recvfrom(buffer_size)
            continue
        window_size, x = unpack('>HL', data[14:20])
        window_size = int(window_size)
        receive_data = data[20:].decode('ascii')
        des_name = receive_data.split('|')[0]
        order = int(receive_data.split('|')[1])
        #print(order)
        log_file = open(des_name + '_receving_log.txt','w')
        log_file.close()
        ack_data = pack('>HHLLHHL', 10080, 0, 0, 0, 0, window_size, 0)
        clientSocket.sendto(ack_data, clientAddress)
        
        receive_queue = queue.Queue()
        queue_size = buffer_size

        t2 = threading.Thread(target = packet_process , args = (clientSocket, clientAddress , des_name, order, window_size, receive_queue, ))
        t1 = threading.Thread(target = packet_capture , args = (clientSocket, probability, order, buffer_size, receive_queue, des_name, ))

        t1.start()
        t2.start()
        
        
