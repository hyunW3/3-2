#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <omp.h>

#define BILLION  1000000000L
int arr_len;

typedef std::vector<std::string> vec;
vec array;
void msd(vec &a, int lo, int hi, int d){
	std::string temp[hi-lo+1];
	int count[256] = {0,};
	int pos[256]={0,};
	if(hi <= lo + 1) return;
	for(int i=lo; i<hi; ++i){
		try{
			count[int(a[i].at(d)) + 1]++;
		}catch (std::out_of_range& oor) {
			count[0]++;
		}
	}
/*
	for(int k=1; k<256; ++k){
		printf("%d ",count[k]);
		if(k == 89) printf("\n");
	}
	printf("\n");
*/
	for(int k=1; k<256; ++k){
		count[k] += count[k-1];
		pos[k] = count[k];
	}
	//printf("\n");
	int zeros =0;
	for(int i=lo; i<hi; ++i){
		try{
			temp[count[a[i].at(d)]] = a[i];
			count[a[i].at(d)]++;
		}catch (std::out_of_range& oor) {
			temp[zeros] = a[i];
			zeros++;
		}
	}
//	printf("%d to %d\n",lo,hi-1);
	for(int i=0; i<hi-lo; ++i){
		a[i+lo] = temp[i];
	}
	for(int i=1; i<255; ++i){
		if( lo+pos[i+1] > lo+pos[i] + 1) {
			msd(a,lo+pos[i],lo+pos[i+1],d+1);
		}
	}
	
}

void msd_main(vec &a){
	//printf("hi main\n");
	msd(a, 0, a.size(), 0);
}
int main(int argc, char* argv[]){
	// declaration
	struct timespec start,stop;

	//read an input file
	std::ifstream inputfile;
	inputfile.open(argv[1]);
    arr_len = atoi(argv[2]);
	int start_show = atoi(argv[3]);
	int end_show = atoi(argv[4]);
	int num_thread = atoi(argv[5]);
    //printf("%s %d\n",argv[1],arr_len);

	omp_set_num_threads(num_thread);
	omp_set_nested(true);
	for(int i=0; i<arr_len; ++i){
		//inputfile.getline(array[i],20);
		std::string tmp;
		std::getline(inputfile,tmp);
		array.push_back(std::move(tmp));
	}
	inputfile.close();
	//now we measure the time
	clock_gettime(CLOCK_REALTIME, &start);
	msd_main(array);
	//serial radix sort

	// we are done with measuring the time
	clock_gettime(CLOCK_REALTIME, &stop);

	//print out the result
	printf("%d to %d\n",start_show,end_show);
	for(int i=start_show; i<=end_show; ++i){
		std::cout << array[i] <<"\n";
	}
    //std::cout << "\n";
	std::cout << "Elapsed time: " << (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION << " sec" << "\n";

	return 0;
}