#!/bin/sh
date "+%Y%m%d_%H:%M:%S" | tee -a result.txt
make 
make run
tail -n 14 Makefile | head -n 8 >> result.txt
for var in $(seq 6)
do
    tail -n 2 output$var.txt | tee -a result.txt
done
