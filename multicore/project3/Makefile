# mpiexec -np {number_of_Processes} --machinefile hosts.txt \
# ./project3 inputfile.txt Num_of_Gens X_limit Y_limit 
main:
	mpiCC -g -Wall -o mpi_main main.cpp
serial:
	mpiCC -g -Wall -o main_serial.exe ./tmp/main_serial.cpp
test:
	#mpiexec --machinefile hosts.txt -n 2 ./mpi_main ./input_file/life.data.1 10 70 70
	mpiexec --machinefile hosts.txt -np 2 -map-by node ./mpi_main ./input_file/life.data.2 10 202400 202400
	#mpiexec --machinefile hosts.txt -n 2 -map-by node ./mpi_main ./input_file/input2.txt 100 100 100
	#mpiexec --machinefile hosts.txt -n 2 -map-by node ./mpi_main ./input_file/input3.txt 50 512 512
	#mpiexec --machinefile hosts.txt -n 16 -map-by node ./mpi_main ./input_file/input4.txt 200 2048 2048
run :
	mpiexec --machinefile hosts.txt -np 16 ./mpi_main ./input_file/life.data.1 30 100 100 > output1.txt
	mpiexec --machinefile hosts.txt -np 16 ./mpi_main ./input_file/life.data.2 30 100 100 > output2.txt
	mpiexec --machinefile hosts.txt -np 64 ./mpi_main ./input_file/life.data.1 3000 2048 2048 > output3.txt
	mpiexec --machinefile hosts.txt -np 128 ./mpi_main ./input_file/life.data.2 3000 102400 204800 > output4.txt
	mpiexec --machinefile hosts.txt -np 64 -map-by node ./mpi_main ./input_file/life.data.1 3000 2048 2048 > output5.txt
	mpiexec --machinefile hosts.txt -np 128 -map-by node ./mpi_main ./input_file/life.data.2 3000 102400 204800 > output6.txt
single:
	mpiexec --machinefile hosts.txt -np 16 -map-by node ./mpi_main ./input_file/input1.txt 3 15 15 > output_map_by_node.txt
serial_run:
	./main_serial.exe ./input_file/life.data.2 10 2048 2048 
clean:
	rm mpi_main 
	
