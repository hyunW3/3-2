#include <stdlib.h>
#include <stdio.h>
#include <cuda_runtime.h>
#include <unistd.h>
#include <cstring>
#include <cstdint>
#include <ctime>
//#define N 1000000
//#define MAX_VAL 1000000
// error in 1000000000
#define N 1000000000
#define MAX_VAL 1000000000

extern void counting_sort(int arr[], int, int);

int main()
{
  int* array = new int[N];

  for(int i=0;i<N;i++){
      array[i] = rand()%MAX_VAL;
      //printf("%02d ",array[i]);
  }

//  printf("\n");
  struct timespec start, end;

  clock_gettime(CLOCK_MONOTONIC, &start);
  counting_sort(array, N, MAX_VAL);
  clock_gettime(CLOCK_MONOTONIC, &end);
/*
  printf("sorted array\n");
  for(int i=0; i<N; i++){
      printf("%02d ",array[i]);
  }
  printf("\n");
*/

  for(int i=0;i<N-1;i++){
      if( array[i] > array[i+1]){
          printf("Not sorted\n");
          exit(1);
      }
  }
  printf("Sorted\n");
  uint64_t elapsed = (end.tv_nsec - start.tv_nsec) + (end.tv_sec - start.tv_sec)*1000000000;
  printf("time(us): %lu\n", elapsed/1000);
  
}
