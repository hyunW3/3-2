#!/bin/sh
#g++ MSD.cpp
#./MSD.exe input_copy.txt 25 0 25 16
./main.exe EMMA.word 159074 0 159074 16 > output.txt
./main.exe wordlist.10000 10000 0 10000 16 > output_wordlist.10000.txt
./main.exe w2.txt 466550 0 466550 16 > output_w2.txt
#./a.out input.txt 22 0 22 16 > input1.txt
#./a.out input.txt 10 1 5 16 > input2.txt