main: main.cpp
	g++ main.cpp -O2 -Wall -o main.exe  -fopenmp
main_g:
	g++ main.cpp -g -Wall -o main.exe  -fopenmp
run:
	#./c.sh
	./main.exe EMMA.word 159074 0 159074 16 > output.txt
	./main.exe wordlist.10000 10000 0 10000 16 > output_wordlist.10000.txt
	./main.exe w2.txt 466550 0 466550 16 > output_w2.txt
run_test:
	#./main.exe w2.txt 418148 0 4 16 
	./main.exe w2.txt 466550 0 466550 16 > output_w2.txt
	./MSD.exe w2.txt 466550 0 466550 16 > output_single_w2.txt
clean:
	rm main.exe output.txt
gdb:
	gdb ./main.exe
diff:
	diff output_single.txt output.txt | tee time.txt
	diff output_single_wordlist.10000.txt output_wordlist.10000.txt | tee -a time.txt
	diff output_single_w2.txt output_w2.txt | tee -a time.txt
	#cat time.txt
single:
	g++ MSD.cpp -O2 -Wall -o MSD.exe 
single_g:
	g++ MSD.cpp -g -Wall -o MSD.exe 
single_run:
	./MSD.exe EMMA.word 159074 0 159074 16 > output_single.txt
	./MSD.exe wordlist.10000 10000 0 10000 16 > output_single_wordlist.10000.txt
	./MSD.exe w2.txt 466550 0 466550 16 > output_single_w2.txt
	#./MSD.exe w2.txt 466550 0 466550 16