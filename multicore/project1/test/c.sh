#!/bin/sh

name=$1
exe=${name%%'.cpp'}
#echo ${exe}
g++ ${name} -o ${exe}.exe -lpthread
./${exe}.exe ../input/input2_p.txt  8
