import time as t
#import math
import threading, queue
import os
start_time = t.time()
thread_num =10
q = queue.Queue(10)
os.remove("log.txt")
log_file = open("log.txt","a")
#cwd = os.getcwd()
#get process utime
def get_logtime():
	logging_time = t.time() - start_time
	return round(logging_time,2)
def copy(src, dst):
	start_time = get_logtime()
	log_file.write(str(start_time)+" Start copying "+src + " to " + dst+"\n")
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
	#	if(i%1500000 == 0):
	#		t.sleep(0.1)
	f.close()
	new_f.close()
		
class machine (threading.Thread):
	def __init__(self, thread_ID, src, dst):
		threading.Thread.__init__(self)
		self.thread_ID = thread_ID  # thread number
		self.src = src
		self.dst = dst
	def run(self):
		#optional
		#log_file.write("Thread " + str(self.thread_ID) + " started\n")

		#log_file.write(str(start_time)+" Start copying "+self.src + " to " + self.dst+"\n")
		#os.fsync(log_file)
		copy(self.src,self.dst)
		q.put(self.thread_ID)
		end_time = get_logtime()
		#print("logging time : {0}".format(end_time))
		log_file.write(str(end_time)+" " + self.dst+ " is copied completely\n")
		t.sleep(0)
thread =[]
def main():
	for i in range(1,11):
		q.put(i)
	while True:
		src = input("Input the file name: ")
		if src == "exit":
			for m in thread:
				m.join()
			return 0;
		elif(os.path.isfile("./"+src)):
			dst = input("Input the new name: ")
			m = machine(q.get(),src,dst)
			m.setDaemon(True)
			m.start()
			thread.append(m)	
		#	m.join()
		else:
			print("Wrong filename:%s" %(src))
		
	log_file.close()
#copy("files/pdf.pdf","pdf1.pdf")	
#copy("files/video.mp4","pdf.mp4")
if __name__ == "__main__":
	main()
