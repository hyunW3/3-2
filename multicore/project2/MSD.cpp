#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

#define BILLION  1000000000L
int arr_len;

typedef std::vector<std::string> vec;
vec a;
void msd( int lo, int hi, unsigned int d){
	//printf("hi-1 %d %d %d\n",lo,hi,d);
	//std::string temp[hi-lo+1];
	vec temp;
	temp.resize(hi-lo+1);
	//printf("hi-2 \n");
	int count[256] = {0,};
	int pos[256]={0,};
	int tmp;
	if(hi <= lo + 1) return;
	//printf("hi 0 \n");
	for(int i=lo; i<hi; ++i){
		//std::cout << a[i].length() <<" ";
		if(static_cast<unsigned int>(a[i].length()) > d){
			count[a[i][d] + 1]++;
		} else count[0]++;
	}
	//printf("hi 1 \n");
	for(int k=1; k<256; ++k){
		for(int j=0; j<=k; j++){
			pos[k] += count[j];
		}
	}
	for(int k=1; k<256; ++k){
		count[k] = pos[k];
	}
	//printf("hi 2 \n");
	//printf("\n");
	int zeros =0;
	for(int i=lo; i<hi; ++i){
		if(static_cast<unsigned int>(a[i].length()) > d){
			tmp = a[i][d];
			temp[count[tmp]] = a[i];
			//strcpy(temp[count[tmp]],a[i].c_str());
			count[tmp]++;
		} else {
			temp[zeros] = a[i];
			//strcpy(temp[count[zeros]],a[i].c_str());
			zeros++;
		}
	}
	//printf("hi 3 \n");
//	printf("%d to %d\n",lo,hi-1);
	for(int i=0; i<hi-lo; ++i){
		a[i+lo] = temp[i];
	}
	
	//printf("hi 4 \n");
	for(int i=1; i<255; ++i){
		if( lo+pos[i+1] > lo+pos[i] + 1) {
			msd(lo+pos[i],lo+pos[i+1],d+1);
		}
	}
	
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
    //printf("%s %d\n",argv[1],arr_len);
	std::string tmp;
	a.clear();
	for(int i=0; i<arr_len; ++i){
		//inputfile.getline(array[i],20);
		std::getline(inputfile,tmp);
		a.push_back(std::move(tmp));
	}
	inputfile.close();
	//now we measure the time
	//std::cout << array.size() <<"\n";
	clock_gettime(CLOCK_REALTIME, &start);
	msd(0, arr_len, 0);
	//serial radix sort

	// we are done with measuring the time
	clock_gettime(CLOCK_REALTIME, &stop);

	//print out the result
	printf("%d to %d\n",start_show,end_show);
	for (vec::const_iterator i = a.begin(); i != a.end(); ++i){
    	std::cout << *i << '\n';
	}
	/*
	for(int i=start_show; i<=end_show; ++i){
		std::cout << a[i] <<"\n";
	}
	*/
    //std::cout << "\n";
	std::cout << "Elapsed time: " << (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION << " sec" << "\n";

	return 0;
}