skiplist: main.cpp
	g++ main.cpp -o main.exe  -fopenmp
run:
	./c.sh
clean:
	rm main.exe output.txt
gdb:
	gdb ./main.exe
single:
	g++ MSD.cpp -o MSD.exe 
single_run:
	./MSD.exe EMMA.word 159074 0 159074 16 > output_single.txt