#!/bin/sh
name=input/make_input.c
#echo ${name}
exe=${name%%'.c'}
gcc ${name} -o ${exe}.exe
./${exe}.exe
