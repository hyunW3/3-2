from datetime import datetime, timedelta
import time

start = datetime.now().strftime('%a, %d %b %Y %H:%M:%S KST')
time.sleep(1)
end = datetime.now().strftime('%a, %d %b %Y %H:%M:%S KST')
end = datetime.strptime(end,'%a, %d %b %Y %H:%M:%S KST')
start = str(start)
start = datetime.strptime(start,'%a, %d %b %Y %H:%M:%S KST')
print(start )
print(type(start))
print(type(end))
print((end-start).seconds)
