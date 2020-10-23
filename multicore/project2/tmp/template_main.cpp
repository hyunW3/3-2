#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <omp.h>

#define BILLION  1000000000L

char array[100000]={0,};

int main(int argc, char* argv[]){
	// declaration
	struct timespec start,stop;

	//read an input file
	std::ifstream inputfile;
	inputfile.open(argv[1]);
	for(int i=0; i<atoi(argv[2]); ++i){
		inputfile >> array[i];
	}
	inputfile.close();

	//now we measure the time
	clock_gettime(CLOCK_REALTIME, &start);

	//OpenMP initialization
	omp_set_num_threads(atoi(argv[5]));

	//parallel radix sort

	// we are done with measuring the time
	clock_gettime(CLOCK_REALTIME, &stop);

	//print out the result
	for(int i=0; i<atoi(argv[2]); ++i){
		std::cout << array[i] <<"\n";
	}
	std::cout << "Elapsed time: " << (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION << " sec" << "\n";

	return 0;
}