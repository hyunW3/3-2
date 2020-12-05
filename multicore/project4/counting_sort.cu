#include <cuda.h>
int nBlk,nTid;
__device__ int position[10]={0,};
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
   //cuda_sort<<<1,size,size*sizeof(int)>>>(arr_d,histogram_d, size, max_val);
   cuda_sort<<<1,size>>>(arr_d,histogram_d, size, max_val);
   // copy to host & finsh
   cudaMemcpy(arr,arr_d,size*sizeof(int),cudaMemcpyDeviceToHost);
   cudaFree(arr_d); cudaFree(histogram_d);
}
__global__ void cuda_sort(int* arr_d,int* histogram_d, int size, int max_val){
   //extern __shared__ int position[]; // https://developer.nvidia.com/gpugems/gpugems3/part-vi-gpu-computing/chapter-39-parallel-prefix-sum-scan-cuda
   int i = threadIdx.x + blockDim.x * blockIdx.x;
   //__device__ int histogram_d[max_val];
   int off = 1;
   if(i<size){
      atomicAdd(&histogram_d[arr_d[i]],1);
   }
   __syncthreads();
   // https://www.eecs.umich.edu/courses/eecs570/hw/parprefix.pdf
   if(i<size){
      position[i] = histogram_d[i];
   }
   for(int stride = size>>1; stride>0; stride >>=1){
      __syncthreads();
      if(i<stride){
         position[off*(2*i+2)-1] += position[off*(2*i+1)-1];
      }
      off *=2;
   }
   __syncthreads();
   for(int j=off-1; j<size-1; j++){
      if(i == j){
         position[j+1] += position[j];
      } 
      __syncthreads();
   }
   if(i==0) position[off-1] = 0;
   for(int stride= 1; stride<size; stride *=2){
      off = off>>1;
      __syncthreads();
      if(i<stride){
         int a = off*(2*i+1)-1;
         int b = off*(2*i+2)-1;
         int tmp = position[a];
         position[a] = position[b];
         position[b] += tmp;
      }
   }
   __syncthreads();
   
   if(i<size){
      //arr_d[i] = histogram_d[i];
      //arr_d[i] = position[i];
      for(int j=0; j<max_val; j++){
         for(int k=0; k<histogram_d[j]; k++){
            arr_d[position[j]+k] = j;
         }
      }
   }
   // device code
}
/*
   atomicAdd(&value[I],1);
   int index =0;
   int i,j;
   for(i=0; i<max_val; i++){
      histogram_d[i] =0;
   }
   for (i=0; i<size; i++){
      histogram_d[arr_d[i]]++;
   }

   for(i=0; i<max_val; i++){
      for(j=0; j<histogram_d[i]; j++){
         arr_d[index++] = i;
      }
   }
*/

