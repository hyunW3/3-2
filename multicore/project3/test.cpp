#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <string.h>

int main(int argc, char *argv[]){
    MPI_Init(NULL,NULL);
    printf("Hello world!\n");

    MPI_Finalize();
    return 0;
}