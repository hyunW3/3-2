#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#define BILLION  1000000000L

int array[100000]={0,};
int comp(const void *a, const  void *b ){
    if(*(int*)a - *(int*)b>0){
        return true;
    }else return false;
}

int main(int argc, char* argv[]){
	// declaration
	struct timespec start,stop;

	//read an input file
	std::ifstream inputfile;
	inputfile.open(argv[1]);
    int arr_len = atoi(argv[2]);
    //printf("%s %d\n",argv[1],arr_len);
	for(int i=0; i<arr_len; ++i){
		inputfile >> array[i];
	}
	inputfile.close();
    std::cout << "before q sort\n";
	for(int i=0; i<arr_len; ++i){
		std::cout << array[i] <<" ";
	}
    std::cout <<"\n";
	//now we measure the time
	clock_gettime(CLOCK_REALTIME, &start);
    qsort((int*)array,arr_len,sizeof(int),comp);

	//serial radix sort

	// we are done with measuring the time
	clock_gettime(CLOCK_REALTIME, &stop);

	//print out the result
    std::cout << "after q sort\n";
	for(int i=0; i<arr_len; ++i){
		std::cout << array[i] <<" ";
	}
    std::cout << "\n";
	std::cout << "Elapsed time: " << (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION << " sec" << "\n";

	return 0;
}