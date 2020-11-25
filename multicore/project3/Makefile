# mpiexec -np {number_of_Processes} --machinefile hosts.txt \
# ./project3 inputfile.txt Num_of_Gens X_limit Y_limit 
main:
	mpiCC -g -Wall -o mpi_main main.cpp
serial:
	mpiCC -g -Wall -o mpi_main ./tmp/main_serial.cpp
test:
	mpiexec --machinefile hosts.txt -n 2 -map-by node ./mpi_main ./input_file/input1.txt 3 15 15
	#mpiexec --machinefile hosts.txt -n 2 ./mpi_main ./input_file/input2.txt 100 100 100
	#mpiexec --machinefile hosts.txt -n 2 ./mpi_main ./input_file/input3.txt 50 512 512
	#mpiexec --machinefile hosts.txt -n 16 ./mpi_main ./input_file/input4.txt 200 2048 2048
run :
	mpiexec --machinefile hosts.txt -n 2 ./mpi_main ./input_file/input1.txt 3 15 15
	mpiexec --machinefile hosts.txt -n 2 ./mpi_main ./input_file/input2.txt 100 100 100
	mpiexec --machinefile hosts.txt -n 2 ./mpi_main ./input_file/input3.txt 50 512 512
	mpiexec --machinefile hosts.txt -n 16 ./mpi_main ./input_file/input4.txt 200 2048 2048
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
	
