main:
	mpiCC -g -Wall -o mpi_main test.cpp
run :
	mpiexec --machinefile hosts.txt -n 4 ./mpi_main
single:
	mpiexec --machinefile hosts.txt -n 16 -map-by node ./mpi_main
titan1:
	ssh titan1 
titan2:
	ssh titan2 
titan3:
	ssh titan3 
titan4:
	ssh titan4 
	
