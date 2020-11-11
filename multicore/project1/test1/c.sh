#!/bin/sh

name=$1
exe=${name%%'.cpp'}
#echo ${exe}
g++ ${name} skiplist.h -o ${exe}.exe -lpthread
./${exe}.exe ../input/input_4.txt 64
