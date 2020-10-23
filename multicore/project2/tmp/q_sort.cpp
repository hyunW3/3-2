#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>

#define BILLION  1000000000L

char array[100000][20];

int comp(const void *a, const  void *b ){
    return strcmp((char*)a,(char*)b);
}

int main(int argc, char* argv[]){
	// declaration
	struct timespec start,stop;

	//read an input file
	std::ifstream inputfile;
	inputfile.open(argv[1]);
    int arr_len = atoi(argv[2]);
	int start_show = atoi(argv[3]);
	int end_show = atoi(argv[4]);
    printf("%s %d\n",argv[1],arr_len);

	for(int i=0; i<arr_len; ++i){
		inputfile.getline(array[i],20);
		//std::cout << array[i] << "\n";
	}
	inputfile.close();
/*	
    std::cout << "====before q sort====\n";
	for(int i=0; i<arr_len; ++i){
		std::cout << array[i] <<"\n";
	}
    //std::cout <<"\n";
*/
	//now we measure the time
	clock_gettime(CLOCK_REALTIME, &start);
    qsort(array,arr_len,sizeof(char)*20,comp);

	//serial radix sort

	// we are done with measuring the time
	clock_gettime(CLOCK_REALTIME, &stop);

	//print out the result
    //std::cout << "====after q sort====\n";
	printf("%d to %d\n",start_show,end_show);
	for(int i=start_show; i<=end_show; ++i){
		std::cout << array[i] <<"\n";
	}
    //std::cout << "\n";
	std::cout << "Elapsed time: " << (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION << " sec" << "\n";

	return 0;
}