#!/bin/sh

name=$1
exe=${name%%'.cpp'}
#echo ${exe}
g++ ${name} -o ${exe}.exe -O2 -lpthread

echo "8 thread"
echo "\n" >> time_log
date >> time_log
echo "8" >> time_log
./${exe}.exe ../input/input_4.txt  8 > tmp.txt
tail -n 1 tmp.txt >> time_log

echo "16 thread"
echo "16" >> time_log
./${exe}.exe ../input/input_4.txt  16 > tmp.txt
tail -n 1 tmp.txt >> time_log

echo "64 thread"
echo "64" >> time_log
./${exe}.exe ../input/input_4.txt  64 > tmp.txt
tail -n 1 tmp.txt >> time_log
 