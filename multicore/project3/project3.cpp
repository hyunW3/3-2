#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <string.h>
#define ALIVE 1
#define DEAD 0

int X_limit,Y_limit,x_range;

__inline
bool row_check(int row,int rank){
    return ((rank*x_range <=row) && (row < (rank+1)* x_range));
}
int main(int argc, char *argv[]){
    int size,rank,num_Gen;
//    double local_start,local_finish,local_elapsed,elapsed;
    int dx[3] = {-1,0,1};
    int dy[3] = {-1,0,1};
    int idx_x,idx_y;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

//    local_start = MPI_Wtime();
///  get argument
    if(argc != 5){
        perror("exit:wrong parameter number\n");
        MPI_Finalize();
        return 1;
    }
    num_Gen = atoi(argv[2]);
    X_limit = atoi(argv[3]);
    Y_limit = atoi(argv[4]);
    x_range = (int)(X_limit/size);
    MPI_Request *req = (MPI_Request *)malloc(sizeof(MPI_Request)*4);
    MPI_Status *status = (MPI_Status *)malloc(sizeof(MPI_Status)*4);
    char** alive_map = new char*[x_range+2];
    char** i_cell = new char*[x_range+2];
    // up & down, 1~x_range use it and 0  & x_range is for communication
    // 1~x_range * 0~Y_limit-1
    for(int i=0; i<x_range+2; i++){
        alive_map[i] = new char[Y_limit];
        memset(alive_map[i],0,Y_limit*sizeof(char));
        i_cell[i] = new char[Y_limit]; 
        memset(i_cell[i],0,(Y_limit)*sizeof(char));
    }
//    printf("rank(%d) - %s %d %d %d\n",rank,argv[1],num_Gen,X_limit,Y_limit);
///  get input
    //FILE* f = fopen(filename,"r");
    FILE* f = fopen(argv[1],"r");
    if (f == NULL) perror("Error in opening file\n");
    
    int row,col;
    while (fscanf(f,"%d %d\n",&row,&col) != EOF) {
        //printf("%d %d\n",row,col);
        if( row_check(row,rank) ){
            i_cell[row-rank*x_range+1][col] = ALIVE;
        }
    }
    fclose(f);
    
/// for test whether input get well
/*
    if (rank == 0){ // for test whether input get well
        for(int i=0; i<X_limit; i++){
	        for(int j=0; j<Y_limit; j++){ // i,j
                if(i_cell[i][j] == DEAD){
                    printf(".");
                }else printf("#");
                //printf("%d",i_cell[i*X_limit+j]); // for test whether input get well
            }
            printf("\n");
        }
        printf("==================\n");
    }
*/
/* error
        memset(i_cell[0],0,Y_limit);
        memset(i_cell[x_range+1],0,Y_limit);
        memset(alive_map,0,x_range*Y_limit);
*/
/// parallel version 
    //for num of Gen
    //if (rank == 0) printf("x_range : %d\n",x_range);
    for(int g=0; g<num_Gen; g++){
    
        if(rank != size-1) {
            MPI_Isend(&i_cell[x_range][0],Y_limit,MPI_CHAR,rank+1,0,MPI_COMM_WORLD,&req[0]);
            MPI_Irecv(&i_cell[x_range+1][0],Y_limit,MPI_CHAR,rank+1,x_range+1,MPI_COMM_WORLD,&req[3]);
        }
        if(rank != 0) {
            MPI_Irecv(&i_cell[0][0],Y_limit,MPI_CHAR,rank-1,0,MPI_COMM_WORLD,&req[1]);
            MPI_Isend(&i_cell[1][0],Y_limit,MPI_CHAR,rank-1,x_range+1,MPI_COMM_WORLD,&req[2]);
        }
    
        if( rank != size-1){
            MPI_Wait(&req[0], &status[0]);  
            MPI_Wait(&req[3], &status[3]); 
        }
        if(rank != 0) {
            MPI_Wait(&req[1], &status[1]);  
            MPI_Wait(&req[2], &status[2]); 
        }
        

        for(int i=1; i<=x_range; i++){
	        for(int j=0; j<Y_limit; j++){ // i,j
                // get pos and cal alive or dead neigherhood
                alive_map[i][j]=0;
                for(int n=0; n<3; n++){
                    for(int m=0; m<3; m++){
                        idx_x = i+dx[n];
                        idx_y = j+dy[m];
                        if(idx_x == i && idx_y == j) continue;
                        if(idx_x <0 || idx_x >x_range+2) continue;
                        if(idx_y <0 || idx_y >=Y_limit) continue;
                        //if(i>=34 && j>=69) printf("(%d) %d,%d-> %d %d, %d\n",rank,i,j,idx_x,idx_y,i_cell[idx_x][idx_y]);
                        if(i_cell[idx_x][idx_y] == ALIVE){ // alive
                            alive_map[i][j] += 1;
                        }
                    }
                }
            }
        }

     
        for(int i=1; i<=x_range; i++){
            for(int j=0; j<Y_limit; j++){
                    if((i_cell[i][j] ==ALIVE) && alive_map[i][j]  ==2){
                        i_cell[i][j] = ALIVE;
                    } else if(alive_map[i][j] ==3){
                        i_cell[i][j]  = ALIVE;
                    } else {    
                        i_cell[i][j]  = DEAD;
                    }
            }
        }
        if (g != num_Gen-1){
            for(int i=0; i<X_limit; i++){
                //MPI_Allreduce(tmp_i_cell[i],i_cell[i],Y_limit,MPI_CHAR,MPI_LOR,MPI_COMM_WORLD);
                //MPI_Allreduce(&tmp_i_cell,&i_cell,X_limit*Y_limit,MPI_CHAR,MPI_MAX,MPI_COMM_WORLD);
            }
        } else {
            // print cell location
            for(int i=1; i<=x_range; i++){
                for(int j=0; j<Y_limit; j++){
                    if(i_cell[i][j]  == ALIVE){
                        printf("%d %d\n",(i-1)+rank*x_range,j);
                    }
                }
            }
        }
    /*
        if(rank == 0){
            for(int i=0; i<X_limit; i++){
                for(int j=0; j<Y_limit; j++){
                    if(i_cell[i*Y_limit+j] == DEAD){
                        printf(".");
                    }else printf("#");
                }
                printf("\n");
            }
            printf("==================\n");
        }
    */

    } //end for(int g=0; g<num_Gen)
 
 /*
	local_finish = MPI_Wtime();
	local_elapsed = local_finish - local_start;
	MPI_Reduce(&local_elapsed,&elapsed,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
*/
  
    
    for(int i=0; i<x_range; i++){
        delete[] alive_map[i];
        delete[] i_cell[i];
    }
    for(int i=x_range; i<x_range+2; i++){
        delete[] i_cell[i];
    }
    delete[] alive_map;
    delete[] i_cell;
    free(req);
    free(status);
    
/// finish program
    
    MPI_Finalize();
//    if(!rank) printf("Elapsed time    : %.3f seconds\n\n",elapsed);
    return 0;
}

