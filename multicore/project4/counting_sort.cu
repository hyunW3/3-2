#include <cuda.h>
int nBlk,nTid;
__global__ void cuda_sort(int* arr_d,int* histogram_d, int size, int max_val);
__host__ void counting_sort(int arr[], int size, int max_val)
{
   nTid = 512;
   nBlk = size/nTid;
   if(nBlk * nTid < size){
      nBlk +=1;
   }
   int * arr_d;
   int* histogram_d;
   cudaMalloc((void**)&arr_d,size*sizeof(int));
   cudaMemcpy(arr_d,arr,size*sizeof(int),cudaMemcpyHostToDevice);
   cudaMalloc((void**)&histogram_d,max_val*sizeof(int));
   cudaMemset((void**)&histogram_d,0,max_val*sizeof(int));
   //cuda_sort<<<nBlk,nTid>>>(arr_d,histogram_d, size, max_val);
   cuda_sort<<<1,size,size*sizeof(int)>>>(arr_d,histogram_d, size, max_val);
   // copy to host & finsh
   cudaMemcpy(arr,arr_d,size*sizeof(int),cudaMemcpyDeviceToHost);
   cudaFree(arr_d); cudaFree(histogram_d);
}
__global__ void cuda_sort(int* arr_d,int* histogram_d, int size, int max_val){
   extern __shared__ int position[]; 
   int i = threadIdx.x + blockDim.x * blockIdx.x;
   //__device__ int histogram_d[max_val];
   if(i<size){
      atomicAdd(&histogram_d[arr_d[i]],1);
   }
   __syncthreads();
   if(i<max_val){
      position[i] = histogram_d[i-1];
      for(int j=0; j<i-1; j++){
         position[i] += histogram_d[j];
      }
   }
   __syncthreads();
   if(i<max_val){
   //   arr_d[i] = histogram_d[i];
   //   arr_d[i] = position[i];
   for(int j=0; j<histogram_d[i]; j++){
      arr_d[position[i]+j] = i;
   }
   
   }
   // device code
}


