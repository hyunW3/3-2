#!/bin/sh

name=$1
exe=${name%%'.cpp'}
echo ${exe}
g++ ${name} -o ${exe}.exe 
./${exe}.exe input.txt 
