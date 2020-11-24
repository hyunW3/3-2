#include <mpi.h>
#include <stdio.h>

int main (int argc, char *argv[]){
int nprocs, myrank ;
int ndims, newprocs, newrank;
MPI_Comm newcomm;
int dimsize[2], periods[2], reorder;
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
ndims = 2; dimsize[0] = 3; dimsize[1] = 2;
periods[0] = 1; periods[1] = 0; reorder = 1;
MPI_Cart_create(MPI_COMM_WORLD,ndims,dimsize,periods,reorder, &newcomm);
MPI_Comm_size(newcomm, &newprocs);
MPI_Comm_rank(newcomm, &newrank);
printf( "%d" , myrank); printf( " newcomm= %d" , newcomm);
printf( " newprocs= %d" , newprocs); printf( " newrank= %d" , newrank);
printf( "\n" );
MPI_Finalize();
return 0;
}