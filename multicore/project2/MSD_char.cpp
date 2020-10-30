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

//typedef std::vector<char *> vec;
//vec a;
char** a;
void msd( int lo, int hi, unsigned int d){
	printf("hi-1 %d %d %d\n",lo,hi,d);
	char temp[hi-lo+1][30];
	printf("hi-2 \n");
	int count[256] = {0,};
	int pos[256]={0,};
	int tmp;
	if(hi <= lo + 1) return;
	printf("hi 0 \n");
	for(int i=lo; i<hi; ++i){
		//std::cout << a[i];
		//printf("%s\n",a[i][d]);
		//std::cout << a[i].length() <<" ";
		if((strlen(a[i])) > d){
			count[a[i][d] + 1]++;
		} else count[0]++;
	}
	//printf("hi 1 \n");
	for(int k=1; k<256; ++k){
		count[k] += count[k-1];
		pos[k] = count[k];
	}
	//printf("hi 2 \n");
	//printf("\n");
	int zeros =0;
	for(int i=lo; i<hi; ++i){
		if((strlen(a[i])) > d){
			tmp = a[i][d];
			//temp[count[tmp]] = a[i];
			strcpy(temp[count[tmp]],a[i]);
			count[tmp]++;
		} else {
			//temp[zeros] = a[i];
			strcpy(temp[count[zeros]],a[i]);
			zeros++;
		}
	}
	//printf("hi 3 \n");
//	printf("%d to %d\n",lo,hi-1);
	for(int i=0; i<hi-lo; ++i){
		strcpy(a[i+lo],temp[i]);
		//a[i+lo] = temp[i];
	}
	
	//printf("hi 4 \n");
	for(int i=1; i<255; ++i){
		if( lo+pos[i+1] > lo+pos[i] + 1) {
			msd(lo+pos[i],lo+pos[i+1],d+1);
		}
	}
	
}

void msd_main(){
	//printf("hi main\n");
	msd(0, arr_len, 0);
}
int main(int argc, char* argv[]){
	// declaration
	struct timespec start,stop;
	int line_len = 0;
	size_t line_buf_size=0;
	//read an input file
	
	FILE *fp = fopen(argv[1],"r");
    arr_len = atoi(argv[2]);
	int start_show = atoi(argv[3]);
	int end_show = atoi(argv[4]);
	//printf("arr_len : %d\n",arr_len);
	a = (char**)malloc(sizeof(char*)*(arr_len+1));
	for(int i=0; i<arr_len; i++){
		a[i] = (char*)malloc(sizeof(char)*30);
	}
	//a.reserve(arr_len);
    //printf("%s %d\n",argv[1],arr_len);
	for(int i=0; i<arr_len; ++i){
		line_len = getline(&a[i],&line_buf_size,fp);
		//printf("%d - %s",line_len,line_buf);
		//a[i][line_len-1] = '\0';
		//printf("%s",a[i]);
	}
	//line_buf = NULL;
	fclose(fp);
	
	//while(1);
	//now we measure the time
	//std::cout << array.size() <<"\n";
	clock_gettime(CLOCK_REALTIME, &start);
	msd_main();
	//serial radix sort

	// we are done with measuring the time
	clock_gettime(CLOCK_REALTIME, &stop);

	//print out the result
	//printf("%d to %d\n",start_show,end_show);
	/*
	for (vec::const_iterator i = a.begin(); i != a.end(); ++i){
    	std::cout << *i << '\n';
	}
	for(int i=start_show; i<=end_show; ++i){
		std::cout << a[i] <<"\n";
	}
	*/
    //std::cout << "\n";
	std::cout << "Elapsed time: " << (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION << " sec" << "\n";

	return 0;
}