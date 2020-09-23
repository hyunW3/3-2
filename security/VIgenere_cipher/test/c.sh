#!/bin/sh
rm result.txt
rm *.exe

gcc -o crash_cipher.exe crash_cipher.c &&

gcc -o make_cipher_0.exe make_cipher_0.c &&
./make_cipher_0.exe &&
./crash_cipher.exe
gcc -o make_cipher_1.exe make_cipher_1.c &&
./make_cipher_1.exe &&
./crash_cipher.exe
gcc -o make_cipher_2.exe make_cipher_2.c &&
./make_cipher_2.exe &&
./crash_cipher.exe
gcc -o make_cipher_3.exe make_cipher_3.c&&
./make_cipher_3.exe &&
./crash_cipher.exe
gcc -o make_cipher_4.exe make_cipher_4.c &&
./make_cipher_4.exe &&
./crash_cipher.exe
gcc -o make_cipher_5.exe make_cipher_5.c &&
./make_cipher_5.exe &&
./crash_cipher.exe
gcc -o make_cipher_6.exe make_cipher_6.c &&
./make_cipher_6.exe &&
./crash_cipher.exe
gcc -o make_cipher_7.exe make_cipher_7.c &&
./make_cipher_7.exe &&
./crash_cipher.exe
gcc -o make_cipher_8.exe make_cipher_8.c &&
./make_cipher_8.exe &&
./crash_cipher.exe
gcc -o make_cipher_9.exe make_cipher_9.c &&
./make_cipher_9.exe &&
./crash_cipher.exe
gcc -o make_cipher_10.exe make_cipher_10.c &&
./make_cipher_10.exe &&
./crash_cipher.exe

echo "TEST whether different"
diff idel_result.txt result.txt
#./crash_cipher_0.exe
