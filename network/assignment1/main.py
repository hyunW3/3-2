import time as t
#import math
import threading, queue
import os
start_time = t.time()
thread_num =11
q = queue.Queue(11)

#cwd = os.getcwd()
#get process utime
def get_logtime():
	logging_time = t.time() - start_time
	return round(logging_time,2)
def copy(src, dst):
	start_time = get_logtime()
	log_file = open("log.txt","a")
	log_file.write(str(start_time).ljust(8)+" Start copying "+src + " to " + dst+"\n")
	log_file.close()
	f = open(src,'rb')
	new_f = open(dst,'wb')
	
	f.seek(0,2)
	fsize = f.tell()
	#print("file size :{0}".format(fsize))
	f.seek(0)
	i =0
	while i < fsize: 
		data = f.read(10000)
		new_f.write(data)
		i += 10000
	f.close()
	new_f.close()
	end_time = get_logtime()
	log_file = open("log.txt","a")
	log_file.write(str(end_time).ljust(8)+" " + dst+ " is copied completely\n")
	log_file.close()
		
class machine (threading.Thread):
	def __init__(self, thread_ID, src, dst):
		threading.Thread.__init__(self)
		self.thread_ID = thread_ID  # thread number
		self.src = src
		self.dst = dst
	def run(self):
		copy(self.src,self.dst)
		q.put(self.thread_ID)
		t.sleep(0)
thread =[]
def main():
	log_file = open("log.txt","w")
	log_file.close()
	for i in range(1,12):
		q.put(i)
	while True:
		src = input("Input the file name: ")
		if src == "exit":
			if thread:
				print("waiting for threads finished")
			for m in thread:
				m.join()
			return 0;
		elif(os.path.isfile("./"+src)):
			dst = input("Input the new name: ")
			print()
			m = machine(q.get(),src,dst)
			m.setDaemon(True)
			m.start()
			thread.append(m)	
		#	m.join()
		else:
			print("Wrong filename:%s" %(src))
		
	

if __name__ == "__main__":
	main()
