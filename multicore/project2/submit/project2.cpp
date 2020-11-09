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
int num_thread;
typedef std::vector<std::string> vec;
vec a;
void msd(int lo, int& hi, unsigned int d){
	//std::string temp[hi-lo+1];
	vec temp;
	temp.resize(hi-lo+1);
	int count[256] = {0,};
	int tmp;
	int zeros =0;
	if(hi <= lo + 1) return;
	#pragma omp parallel if(hi-lo>100000) private(tmp) shared(count,zeros,temp)  //shared(a,lo,hi)
	{
	#pragma omp for //if(d<2)
	for(int i=lo; i<hi; ++i){
		if(a[i].length() > d){
			//#pragma omp atomic
			//count[a[i].at(d) + 1]++;
			tmp = a[i][d];
			#pragma omp atomic
			count[tmp]++;
		} else {
			#pragma omp atomic
			count[0]++;
		}
	}
	}
	for(int k=1; k<256; ++k){
		count[k] += count[k-1];
	}
	//#pragma omp for schedule(dynamic,1)
	for(int i=lo; i<hi; ++i){
		if(a[i].length() > d){
			//tmp = a[i].at(d);
			//tmp = a[i][d]-1;
			temp[count[a[i][d]-1]++] = a[i];
			//count[tmp]++;
		} else {
			temp[zeros++] = a[i];
			//zeros++;
		}
	}
//	printf("%d to %d\n",lo,hi-1);
	#pragma omp parallel shared(count,temp)  //shared(a,lo,hi)
	{
	int size = hi-lo;
	#pragma omp for 
	for(int i=0; i<size; ++i){
		a[i+lo] = temp[i];
	}
	//#pragma omp for schedule(dynamic,4)
	#pragma omp single // schedule(dynamic,4)
	for(int i=0; i<255; ++i){
		int front = lo+count[i];
		int back = lo+count[i+1];
		if( back > front + 1) {
			#pragma omp task 
			msd(front,back,d+1);
		} 
	}
	} // end omp parallel
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
	num_thread = atoi(argv[5]);
    //printf("%s %d\n",argv[1],arr_len);
	a.resize(arr_len+1);
	omp_set_num_threads(num_thread);
	//omp_set_nested(true);  // when add it, the elapsed time is over 20sec
	for(int i=0; i<arr_len; ++i){
		//inputfile.getline(array[i],20);
		std::getline(inputfile,a[i]);
		//std::string tmp;
		//std::getline(inputfile,tmp);
		//a.push_back(std::move(tmp));
	}
	inputfile.close();
	//now we measure the time
	clock_gettime(CLOCK_REALTIME, &start);
	msd(0, arr_len, 0);
	//serial radix sort

	// we are done with measuring the time
	clock_gettime(CLOCK_REALTIME, &stop);

	//print out the result
	for(int i=start_show; i<=end_show; ++i){
		std::cout << a[i] <<"\n";
	}
    //std::cout << "\n";
	std::cout << "Elapsed time: " << (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION << " sec" << "\n";

	return 0;
}