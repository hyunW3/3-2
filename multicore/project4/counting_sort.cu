#include <cuda.h>

__host__ void counting_sort(int arr[], int size, int max_val)
{
   // fill in 
   int * arr_d,histogram_d;
   int i,j;
   int index =0;
   cudaMalloc((void**)&arr_d,size);
   cudaMemcpy(arr_d,arr,size,cudaMemcpyHostToDevice);
   cudaMalloc((void**)&histogram_d,max_val);
   //for(i=0; i<max_val; i++){
      histogram_d[i] =0;
   //}
   //for (i=0; i<size; i++){
      histogram_d[arr_d[i]]++;
   //}

   //for(i=0; i<max_val; i++){
   //   for(j=0; j<histogram_d[i]; j++){
         arr_d[index++] = i;
   //   }
   //}

   // copy to host & finsh
   cudaMemcpy(arr,arr_d,size,cudaMemcpyDeviceToHost);
   cudaFree(arr_d); cudaFree(histogram_d);
}

