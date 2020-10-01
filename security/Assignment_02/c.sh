#!/bin/sh
name=$1
exe=${name%%'.c'}
gcc ${name} -o ${exe}.exe -lcrypto
#gcc -Wall ${name} -o ${exe}.exe -lcrypto
./${exe}.exe
