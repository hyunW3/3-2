# mpiexec -np {number_of_Processes} --machinefile hosts.txt \
# ./project3 inputfile.txt Num_of_Gens X_limit Y_limit 
main:
	mpiCC -g -Wall -o mpi_main main.cpp
project3:
	mpiCC -g -Wall -o mpi_main project3.cpp
serial:
	mpiCC -g -Wall -o mpi_main ./tmp/project3_1125_0540.cpp
test:
	#mpiexec --machinefile hosts.txt -np 10 ./mpi_main ./input2.txt 10 70 70
	mpiexec --machinefile hosts.txt -np 10 ./mpi_main ./input_file/life.data.1 1 70 70 > output1.txt
	mpiexec --machinefile hosts.txt -np 10 ./mpi_main ./input_file/life.data.1 2 70 70 > output2.txt
	mpiexec --machinefile hosts.txt -np 10 ./mpi_main ./input_file/life.data.1 3 70 70 > output3.txt
	mpiexec --machinefile hosts.txt -np 10 ./mpi_main ./input_file/life.data.1 4 70 70 > output4.txt
	mpiexec --machinefile hosts.txt -np 10 ./mpi_main ./input_file/life.data.1 5 70 70 > output5.txt
	#mpiexec --machinefile hosts.txt -np 36 -map-by node ./mpi_main ./input_file/life.data.1 1 72 108 > output_1.txt
	#mpiexec --machinefile hosts.txt -np 2 -map-by node ./mpi_main ./input_file/input2.txt 100 100 100
	#mpiexec --machinefile hosts.txt -np 2 -map-by node ./mpi_main ./input_file/input3.txt 50 512 512
	#mpiexec --machinefile hosts.txt -np 16 -map-by node ./mpi_main ./input_file/input4.txt 200 2048 2048
run :
	mpiexec --machinefile hosts.txt -np 1 ./mpi_main ./input_file/life.data.1 60 900 900 > output1.txt
	mpiexec --machinefile hosts.txt -np 1 ./mpi_main ./input_file/life.data.2 90 900 900 > output2.txt
	mpiexec --machinefile hosts.txt -np 36 ./mpi_main ./input_file/life.data.2 300 10800 10800 > output3.txt
	mpiexec --machinefile hosts.txt -np 36 ./mpi_main ./input_file/life.data.2 600 10800 10800 > output4.txt
	mpiexec --machinefile hosts.txt -np 36 -map-by node ./mpi_main ./input_file/life.data.2 300 10800 10800 > output5.txt
	mpiexec --machinefile hosts.txt -np 36 -map-by node ./mpi_main ./input_file/life.data.2 600 10800 10800> output6.txt
single:
	mpiexec --machinefile hosts.txt -np 16 -map-by node ./mpi_main ./input_file/input1.txt 3 15 15 > output_map_by_node.txt
serial_run:
	./main_serial.exe ./input_file/life.data.2 10 2048 2048 
clean:
	rm mpi_main 
	
