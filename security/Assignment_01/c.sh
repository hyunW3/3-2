#!/bin/sh
gcc -o make_cipher.exe make_cipher.c -lm &&
./make_cipher.exe &&
gcc -o crash_cipher.exe crash_cipher.c -lm &&
./crash_cipher.exe
