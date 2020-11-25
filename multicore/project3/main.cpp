#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <string.h>
#define ALIVE 1
#define DEAD 0

int X_limit,Y_limit;
int get_num_alive(int* i_cell,int x_pos,int y_pos);
int main(int argc, char *argv[]){
    int size,rank,num_Gen;
    double local_start,local_finish,local_elapsed,elapsed;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    local_start = MPI_Wtime();
///  get argument
    if(argc != 5){
        perror("exit:wrong parameter number\n");
        MPI_Finalize();
        return 1;
    }
    num_Gen = atoi(argv[2]);
    X_limit = atoi(argv[3]);
    Y_limit = atoi(argv[4]);
    //int* alive_map = (int*)calloc(Y_limit*X_limit,sizeof(int)); // row * column : X_limit * Y_limit
    int alive_map[X_limit*Y_limit]={0,};
    int i_cell[X_limit*Y_limit]={0,}; // 0~X_limit-1 * 0~Y_limit-1
    int tmp_i_cell[X_limit*Y_limit]={0,};

    memset(i_cell,0,X_limit*Y_limit*sizeof(int));
    memset(tmp_i_cell,0,X_limit*Y_limit*sizeof(int));
    printf("rank(%d) - %s %d %d %d\n",rank,argv[1],num_Gen,X_limit,Y_limit);
///  get input
    //FILE* f = fopen(filename,"r");
    FILE* f = fopen(argv[1],"r");
    if (f == NULL) perror("Error in opening file\n");
    
    int row,col;
    while (fscanf(f,"%d %d\n",&row,&col) != EOF) {
        //printf("%d %d\n",row,col);
        i_cell[row*X_limit+col] =ALIVE;
    }
    fclose(f);
    
/// for test whether input get well

    if (rank == 0){ // for test whether input get well
        for(int i=0; i<Y_limit; i++){
	        for(int j=0; j<X_limit; j++){ // i,j
                if(i_cell[i*X_limit+j] == DEAD){
                    printf(".");
                }else printf("#");
                //printf("%d",i_cell[i*X_limit+j]); // for test whether input get well
            }
            printf("\n");
        }
        printf("==================\n");
    }



/// serial version 
    //for num of Gen
    
    for(int g=0; g<num_Gen; g++){
        for(int i=0; i<Y_limit; i++){
	        for(int j=0; j<X_limit; j++){ // i,j
                // get pos and cal alive or dead neigherhood
                int index = i*X_limit+j;
                if((index)%size == rank){
                    alive_map[index] = get_num_alive(i_cell,i,j);
                    // local_alive_map
                }
            }
        }
        for(int i=0; i<Y_limit; i++){
            for(int j=0; j<X_limit; j++){
                int index = i*X_limit+j;
                if(index%size == rank){ // MPI Divde
                    if((i_cell[index]==ALIVE) && alive_map[index] ==2){
                        tmp_i_cell[index] = ALIVE;
                    } else if(alive_map[index] ==3){
                        tmp_i_cell[index] = ALIVE;
                    } else {    
                        tmp_i_cell[index] = DEAD;
                    }
                } // end if((index)%size == rank)
            }
        }
        MPI_Allreduce((void*)&tmp_i_cell,(void*)&i_cell,X_limit*Y_limit,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
        
        if(rank == 0){
            for(int i=0; i<Y_limit; i++){
                for(int j=0; j<X_limit; j++){
                    if(i_cell[i*X_limit+j] == DEAD){
                        printf(".");
                    }else printf("#");
                }
                printf("\n");
            }
            printf("==================\n");
        }
        
        
    } //end for(int g=0; g<num_Gen)
    
	local_finish = MPI_Wtime();
	local_elapsed = local_finish - local_start;
	MPI_Reduce(&local_elapsed,&elapsed,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);


	if(!rank)   printf("Elapsed time    : %.3f seconds\n\n",elapsed);

/// finish program
    
    MPI_Finalize();
    return 0;
}

int get_num_alive(int* i_cell,int x_pos,int y_pos){
    int dx[3] = {-1,0,1};
    int dy[3] = {-1,0,1};
    int count =0;
    int idx_x,idx_y;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            idx_x = x_pos+dx[i];
            idx_y = y_pos+dy[j];
            if(idx_x == x_pos && idx_y == y_pos) continue;
            if(idx_x <0 || idx_x >=X_limit) continue;
            if(idx_y <0 || idx_y >=Y_limit) continue;
            if(i_cell[idx_x*X_limit+idx_y]==ALIVE){ // alive
                count +=1;
            }
        }
    }
    return count;
}
