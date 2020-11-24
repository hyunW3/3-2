#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
    int size;
    int rank;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("size :%d, rank: %d\n",size,rank);

    char buf[100];
    gethostname(buf,100);
    printf("hostname : %s\n",buf);

    MPI_Finalize();
    return 0;
}