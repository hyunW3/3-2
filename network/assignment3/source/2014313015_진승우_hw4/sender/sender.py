from socket import *
import threading
from struct import *
import time 


def timeout_check(serverSock, order, timeout, packet_data, IP, window_size, src_name):
    global oldest, send, packet_state, packet_timeout, start
    while True:
        with thread_lock:
            if oldest == order :
                break
            elif oldest == send :
                continue
            elif (time.time() - float(packet_timeout[oldest])) >= float(timeout) :
                ta = threading.Thread(target = log_file_write, args = (src_name, oldest, time.time() - start, 4))
                ta.start()
                ta.join()
                #log_file_write(src_name, oldest, time.time() - start, 4)
                packet_retransmit = pack('>HHLLHHL', 0, 10080, oldest, 0, 0, window_size, 0)
                packet_retransmit = packet_retransmit + packet_data[oldest]
                serverSocket.sendto(packet_retransmit, (IP, 10080))
                if oldest != 0 :
                    packet_state[oldest-1] = 0
                tb = threading.Thread(target = log_file_write, args = (src_name, oldest, time.time() - start, 3))
                tb.start()
                tb.join()
                #log_file_write(src_name, oldest, time.time() - start, 3)
                packet_timeout[oldest] = time.time()


def ack_receiver(serverSocket, order, packet_data, window_size, IP, src_name):
    global oldest, send, packet_state, packet_timeout, start
    #ack = serverSocket.recvfrom(1400)
    while True:
        data = serverSocket.recv(1400)
        x, x, ack_num, x = unpack(">LLLH", data[:14])
        tc = threading.Thread(target = log_file_write, args = (src_name, ack_num, time.time() - start, 0))
        tc.start()
        tc.join()
        #log_file_write(src_name, ack_num, time.time() - start, 0 )
        with thread_lock:
            packet_state[ack_num] += 1
            oldest = ack_num + 1
            if oldest > send :
                send = oldest
            if packet_state[ack_num] == 3 :
                packet_retransmit = pack('>HHLLHHL', 0, 10080, oldest, 0, 0, window_size, 0)
                packet_retransmit = packet_retransmit + packet_data[oldest]
                serverSocket.sendto(packet_retransmit, (IP, 10080))
                packet_state[ack_num] = 0
                td = threading.Thread(target = log_file_write, args = (src_name, oldest - 1, time.time() - start, 2))
                td.start()
                td.join()
                td = threading.Thread(target = log_file_write, args = (src_name, oldest, time.time() - start, 1))
                td.start()
                td.join()
                #log_file_write(src_name, oldest, time.time() - start, 2)
             
        if oldest == order :
            tk = threading.Thread(target = log_file_write, args = (src_name, oldest, time.time() - start, 5))
            tk.start()
            tk.join()
            break


def send(serverSocket, IP, order, packet_data, window_size, src_name):
    global oldest, send, packet_state, packet_timeout, start
    oldest = 0
    send = 0
    start = 0
    packet_state = []
    packet_timeout = dict()
    while True:
        while send - oldest < window_size and oldest != order :
            with thread_lock:
                packet = pack('>HHLLHHL', 0, 10080, send, 0, 0, window_size, 0)
                packet += packet_data[send]
                serverSocket.sendto(packet, (IP, 10080))
                if start == 0:
                    start = time.time()
                te = threading.Thread(target = log_file_write, args = (src_name, send, time.time() - start, 1))
                te.start()
                te.join()
                #log_file_write(src_name, send, time.time() - start, 1)
                
                packet_timeout[send] = time.time()
                #print(f"{time.time() - start:.3f} pkt: {send} Sender > Receiver")
                packet_state.append(0)
                send += 1
                if send == order : 
                    break
        if send == order:
            break


def log_file_write(src_name, num, time, sent) :
    #with thread_lock:
        #log_file = open(src_name + '_sending_log.txt','a')
        while True:
            try:
                log_file = open(src_name + '_sending_log.txt','a')
                break
            except PermissionError :
                continue
        delta = "%0.3f"% float(time)
        if sent == 0 :
            log_write = str(str(delta) + " ACK: " + str(num) +" | received\n")
        elif sent == 1:
            log_write = str(str(delta) + " pkt: " + str(num) +" | sent\n")
        elif sent == 2 :
            log_write = str(str(delta) + " pkt: " + str(num) +" | 3 duplicated ACKs\n")
        elif sent == 3:
            log_write = str(str(delta) + " pkt: " + str(num) +" | retransmitted\n")
        elif sent == 4:
            log_write = str(str(delta) + " pkt: " + str(num) +" | timeout since " + "%0.3f"% float(delta) +  "\n")
        elif sent == 5:
            goodput = "%0.3f"% (num/float(time))
            log_write = "\n" + "File transfer is finished." + "\n" + "Throughput: " + str(goodput) + " pkts / sec"                     

        if not log_write == 'ed' :
            log_file.write(log_write)
        log_file.close()


if __name__ == '__main__':
    IP = input('Receiver IP address: ')
    window_size = int(input('window size: '))
    time_out = float(input('timeout (sec): '))
                             
    print('')

    thread_lock = threading.Lock()
    packet_data = dict()
    packet_state = []
    packet_timeout = dict()

    
    src_name = input('file name: ')
    src_file = open(src_name, 'rb')
    chunk_size = 1024
    file_size = 0
    order = 0
    while True:
        file_size_indicator = src_file.read(1380)
        if not file_size_indicator:
            break
        file_size += len(file_size_indicator)
        packet_data[order] = file_size_indicator
        order += 1
    src_file.close()

    log_file = open(src_name + '_sending_log.txt','w')
    log_file.close()
    
    serverPort = 0
    serverSocket = socket(AF_INET,SOCK_DGRAM)
    serverSocket.bind(("",serverPort))

    str_order = str(order)
    
    data = bytearray(src_name, encoding = 'ascii') + bytearray('|', encoding = 'ascii') + bytearray(str_order, encoding = 'ascii')
    data_packet = pack('>HHLLHHL',0, 10080, 0, 0, 0, window_size, 0)
    data_packet = data_packet + data
    serverSocket.sendto(data_packet, (IP, 10080))
    data = serverSocket.recv(1400)
    x, x, ack_num, x = unpack(">LLLH", data[:14])
    while ack_num == -1:
        data = serverSocket.recv(1400)
        x, x, ack_num, x = unpack(">LLLH", data[:14])

    t1 = threading.Thread(target=ack_receiver , args = (serverSocket, order, packet_data,window_size, IP, src_name, ))
    t2 = threading.Thread(target=send, args = (serverSocket, IP, order, packet_data, window_size, src_name,  ))
    t3 = threading.Thread(target=timeout_check, args = (serverSocket, order, time_out, packet_data, IP, window_size, src_name,  ))

    t1.start()
    t2.start()
    t3.start()


    #print('File transfer is finished.')
    #print('Throughput: ')


                
