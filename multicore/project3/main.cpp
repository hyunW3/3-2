#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
    int size,rank,num_Gen,X_limit,Y_limit;
    char* filename;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
///  get argument
    if(argc != 5){
        perror("exit:wrong parameter number\n");
        MPI_Finalize();
        return 1;
    }
    filename = (char*)malloc(sizeof(char)*(strlen(argv[1]+1)));
    strcpy(filename,argv[1]);
    num_Gen = atoi(argv[2]);
    X_limit = atoi(argv[3]);
    Y_limit = atoi(argv[4]);
    char** cell = (char**)calloc(Y_limit,sizeof(char*));
    for(int i=0; i<Y_limit; i++){
        cell[i] = (char*)calloc(X_limit+2,sizeof(char));
    }
//    printf("rank(%d) - %s %d %d %d\n",rank,filename,num_Gen,X_limit,Y_limit);
///  get input
    FILE* f = fopen(filename,"r");
    if (f == NULL) perror("Error in opening file\n");
    
//    if (rank == 0){ // for test whether input get well
        for(int y=0; y<Y_limit; y++){
            fgets(cell[y],X_limit+2,f);
            cell[y][X_limit] = '\0';
//            printf("%d-%s\n",y,cell[y]); // for test whether input get well
        }
//    }
    fclose(f);
    

/// finish program
    free(filename);
    for(int i=0; i<Y_limit; i++){
        free(cell[i]);
    }
    free(cell);
    
    MPI_Finalize();
    return 0;
}