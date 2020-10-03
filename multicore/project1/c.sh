#!/bin/sh

name=$1
exe=${name%%'.c'}
g++ ${name} -o ${exe}.exe -lpthread
./${exe}.exe
