#include <cuda.h>
#include <cmath>
   
int nBlk,nTid;
__global__ void cuda_sort(int* arr_d,int* out, int* histogram_d, int size, int max_val);
__host__ void counting_sort(int arr[], int size, int max_val)
{
   int* arr_d;
   int* out;
   int* histogram_d;
   cudaMalloc((void**)&arr_d,size*sizeof(int));
   cudaMalloc((void**)&out  ,size*sizeof(int));
   cudaMemcpy(arr_d,arr,size*sizeof(int),cudaMemcpyHostToDevice);
   cudaMalloc((void**)&histogram_d,max_val*sizeof(int));
   cudaMemset((void**)&histogram_d,0,max_val*sizeof(int));
   if(size>1024)  cuda_sort<<<ceil(size/1024),1024,size*sizeof(int)>>>(arr_d,out, histogram_d, size, max_val);
   else           cuda_sort<<<1,size,size*sizeof(int)>>>(arr_d,out, histogram_d, size, max_val);
   // copy to host & finsh
   cudaMemcpy(arr,out,size*sizeof(int),cudaMemcpyDeviceToHost);
   cudaFree(arr_d); cudaFree(histogram_d);
   cudaFree(out);
}

__global__ void cuda_sort(int* arr_d,int* out, int* histogram_d, int size, int max_val){
   extern __shared__ int position[]; 
   int i = threadIdx.x + blockDim.x * blockIdx.x;
   //__device__ int histogram_d[max_val];
   if(i<size){
      atomicAdd(&histogram_d[arr_d[i]],1);
   }
   __syncthreads();
   if(i<max_val){
      if(i==0) position[0] = 0;
      else {
         position[i] = histogram_d[i-1];
         for(int j=0; j<i-1; j++){
            position[i] += histogram_d[j];
         }
      }
   }
   __syncthreads();
   if(i<max_val){ 

   for(int j=0; j<histogram_d[i]; j++){
      out[position[i]+j] = i;
   }

   //   memset(&arr_d[position[i]],i,sizeof(int)*histogram_d[i]);
   }
   // device code
}


