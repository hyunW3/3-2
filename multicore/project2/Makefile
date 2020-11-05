
filename := $(shell date "+%Y%m%d_%H:%M:%S" )
main: main.cpp
	g++ main.cpp -O2 -Wall -o main.exe  -fopenmp
	g++ MSD.cpp -O2 -Wall -o MSD.exe 
main_g:
	g++ main.cpp -g -Wall -o main.exe  -fopenmp
run:
	#./c.sh
	./main.exe ./input/EMMA.word 159074 0 159074 16 > ./output/output.txt
	./main.exe ./input/wordlist.10000 10000 0 10000 16 > ./output/output_wordlist.10000.txt
	./main.exe ./input/w2.txt 466550 0 466550 16 > ./output/output_w2.txt
	./main.exe ./input/exampe.txt 325239 0 325239 16 > ./output/output_example.txt
	./main.exe ./input/example6MB.txt 1164968 0 1164968 16 > ./output/output_example6MB.txt
	./main.exe ./input/example_1million.txt 1000000 0 1000000 16 > ./output/output_example_1million.txt
	./main.exe ./input/example_10million.txt 10000000 0 10000000 16 > ./output/output_example_10million.txt
	./main.exe ./input/example_100million.txt 100000000 0 100000000 16 > ./output/output_example_100million.txt
	#./main.exe ./input/example_1000million.txt 1000000000 0 1000000000 16 > ./output/output_example_1000million.txt
run_32:
	./main.exe ./input/example_1million.txt 1000000 0 1000000 32 > ./output/output_example_1million_32.txt
	./main.exe ./input/example_10million.txt 10000000 0 10000000 32 > ./output/output_example_10million_32.txt
	./main.exe ./input/example_100million.txt 100000000 0 100000000 32 > ./output/output_example_100million_32.txt
	./main.exe ./input/w2.txt 466550 0 466550 32 > ./output/output_32_w2.txt
run_core:
	g++ main.cpp -O2 -Wall -o main.exe  -fopenmp
	./main.exe ./input/example_10million.txt 10000000 0 10000000 16 > ./output/output_example_10million.txt
	./main.exe ./input/example_100million.txt 100000000 0 100000000 16 > ./output/output_example_100million.txt
	#./main.exe ./input/example_1000million.txt 1000000000 0 1000000000 16 > ./output/output_example_1000million.txt
	diff ./output/output_single_example_10million.txt ./output/output_example_10million.txt | tee -a time.txt
	diff ./output/output_single_example_100million.txt ./output/output_example_100million.txt | tee -a time.txt
run_test:
	./main.exe ./input/w2.txt 466550 0 466550 16 > ./output/output_w2.txt
	./MSD.exe ./input/w2.txt 466550 0 466550 16 > ./output/output_single_w2.txt
clean:
	rm *.exe 
clean_output:
	rm ./output/*
gdb:
	gdb ./main.exe
diff:
	echo "================================" >> time.txt
	cp main.cpp ./mainfile/main_$(filename).cpp 
	@echo main_$(filename).cpp | tee -a time.txt
	diff ./output/output_single.txt ./output/output.txt | tee -a time.txt
	diff ./output/output_single_wordlist.10000.txt ./output/output_wordlist.10000.txt | tee -a time.txt
	diff ./output/output_single_w2.txt ./output/output_w2.txt | tee -a time.txt
	diff ./output/output_single_example.txt ./output/output_example.txt | tee -a time.txt
	diff ./output/output_single_example6MB.txt ./output/output_example6MB.txt | tee -a time.txt
	diff ./output/output_single_example_1million.txt ./output/output_example_1million.txt | tee -a time.txt
	diff ./output/output_single_example_10million.txt ./output/output_example_10million.txt | tee -a time.txt
	diff ./output/output_single_example_100million.txt ./output/output_example_100million.txt | tee -a time.txt
	diff ./output/output_single_example_1000million.txt ./output/output_example_1000million.txt | tee -a time.txt
	#cat time.txt
diff_one:
	diff ./output/output_single_example_1000million.txt ./output/output_example_1000million.txt 
single:
	g++ MSD.cpp -O2 -Wall -o MSD.exe 
q_sort:
	g++ qsort.cpp -O2 -Wall -o qsort.exe 
single_g:
	g++ MSD.cpp -g -Wall -o MSD.exe 
single_run:
	./MSD.exe ./input/EMMA.word 159074 0 159074 16 > ./output/output_single.txt
	./MSD.exe ./input/wordlist.10000 10000 0 10000 16 > ./output/output_single_wordlist.10000.txt
	./MSD.exe ./input/w2.txt 466550 0 466550 16 > ./output/output_single_w2.txt
	./MSD.exe ./input/exampe.txt 325239 0 325239 16 > ./output/output_single_example.txt
	./MSD.exe ./input/example6MB.txt 1164968 0 1164968 16 > ./output/output_single_example6MB.txt
	./MSD.exe ./input/example_1million.txt 1000000 0 1000000 16 > ./output/output_single_example_1million.txt
	./MSD.exe ./input/example_10million.txt 10000000 0 10000000 16 > ./output/output_single_example_10million.txt
	./MSD.exe ./input/example_100million.txt 100000000 0 100000000 16 > ./output/output_single_example_100million.txt
q_sort_run:
	./qsort.exe  ./input/EMMA.word 159074 0 159074 16 > ./output/output_single.txt
	./qsort.exe ./input/wordlist.10000 10000 0 10000 16 > ./output/output_single_wordlist.10000.txt
	./qsort.exe ./input/w2.txt 466550 0 466550 16 > ./output/output_single_w2.txt
	./qsort.exe ./input/exampe.txt 325239 0 325239 16 > ./output/output_single_example.txt
	./qsort.exe ./input/example6MB.txt 1164968 0 1164968 16 > ./output/output_single_example6MB.txt
	./qsort.exe ./input/example_1million.txt 1000000 0 1000000 16 > ./output/output_single_example_1million.txt
	./qsort.exe ./input/example_10million.txt 10000000 0 10000000 16 > ./output/output_single_example_10million.txt
	./qsort.exe ./input/example_100million.txt 100000000 0 100000000 16 > ./output/output_single_example_100million.txt
	./qsort.exe ./input/example_1000million.txt 1000000000 0 1000000000 16 > ./output/output_single_example_1000million.txt
test:
	@echo My_var is $(filename)
	@echo main_$(filename).cpp