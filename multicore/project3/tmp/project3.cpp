#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <string.h>
#define ALIVE 1
#define DEAD 0

int X_limit,Y_limit;
char get_num_alive(int x_pos,int y_pos);
char** i_cell;
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
    //char alive_map[X_limit*Y_limit]={0,};
    //bool i_cell[X_limit*Y_limit]={0,}; // 0~X_limit-1 * 0~Y_limit-1
    //bool tmp_i_cell[X_limit*Y_limit]={0,};
/*
    char* alive_map = new char[X_limit*Y_limit];
    bool* i_cell = new bool[X_limit*Y_limit];
    bool* tmp_i_cell = new bool[X_limit*Y_limit];
*/  
    char** alive_map = new char*[X_limit];
    i_cell = new char*[X_limit];
    char** tmp_i_cell = new char*[X_limit];
    for(int i=0; i<X_limit; i++){
        alive_map[i] = new char[Y_limit];
        memset(alive_map[i],0,Y_limit*sizeof(char));
        i_cell[i] = new char[Y_limit];
        memset(i_cell[i],0,Y_limit*sizeof(char));
        tmp_i_cell[i] = new char[Y_limit];
        memset(tmp_i_cell[i],0,Y_limit*sizeof(char));
    }
//    printf("rank(%d) - %s %d %d %d\n",rank,argv[1],num_Gen,X_limit,Y_limit);
///  get input
    //FILE* f = fopen(filename,"r");
    FILE* f = fopen(argv[1],"r");
    if (f == NULL) perror("Error in opening file\n");
    
    int row,col;
    while (fscanf(f,"%d %d\n",&row,&col) != EOF) {
        //printf("%d %d\n",row,col);
        i_cell[row][col] = ALIVE;
        //if(i_cell[row][col] == ALIVE) printf("%d %d - %d\n",row,col,i_cell[row][col]);
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


/// parallel version 
    //for num of Gen
    
    for(int g=0; g<num_Gen; g++){
        for(int i=0; i<X_limit; i++){
	        for(int j=0; j<Y_limit; j++){ // i,j
                // get pos and cal alive or dead neigherhood
                int index = i*Y_limit+j;
                if((index)%size == rank){
                    alive_map[i][j] = get_num_alive(i,j);
                    //if(alive_map[i][j] != 0 ) printf("%d %d %d",i,j,alive_map[i][j]);
                    // local_alive_map
                }
            }
        }
        for(int i=0; i<X_limit; i++){
            for(int j=0; j<Y_limit; j++){
                int index = i*Y_limit+j;
                if(index%size == rank){ // MPI Divde
                    if((i_cell[i][j] ==ALIVE) && alive_map[i][j]  ==2){
                        tmp_i_cell[i][j] = ALIVE;
                    } else if(alive_map[i][j] ==3){
                        tmp_i_cell[i][j]  = ALIVE;
                    } else {    
                        tmp_i_cell[i][j]  = DEAD;
                    }
                } // end if((index)%size == rank)
            }
        }
        if (g != num_Gen-1){
            for(int i=0; i<X_limit; i++){
                MPI_Allreduce(tmp_i_cell[i],i_cell[i],Y_limit,MPI_CHAR,MPI_LOR,MPI_COMM_WORLD);
                //MPI_Allreduce(&tmp_i_cell,&i_cell,X_limit*Y_limit,MPI_CHAR,MPI_MAX,MPI_COMM_WORLD);
            }
        } else {
            // print cell location
            /*
            for(int i=0; i<X_limit; i++){
                for(int j=0; j<Y_limit; j++){
                    if(tmp_i_cell[i][j]  == ALIVE){
                        printf("%d %d\n",i,j);
                    }
                }
            }
            */
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
 
	local_finish = MPI_Wtime();
	local_elapsed = local_finish - local_start;
	MPI_Reduce(&local_elapsed,&elapsed,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

  
    for(int i=0; i<X_limit; i++){
        delete[] alive_map[i];
        delete[] i_cell[i];
        delete[] tmp_i_cell[i];
    }
    delete[] alive_map;
    delete[] i_cell;
    delete[] tmp_i_cell;
    
/// finish program
    
    MPI_Finalize();
    if(!rank) printf("Elapsed time    : %.3f seconds\n\n",elapsed);
    return 0;
}

__inline 
char get_num_alive(int x_pos,int y_pos){
    int dx[3] = {-1,0,1};
    int dy[3] = {-1,0,1};
    char count =0;
    int idx_x,idx_y;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            idx_x = x_pos+dx[i];
            idx_y = y_pos+dy[j];
            if(idx_x == x_pos && idx_y == y_pos) continue;
            if(idx_x <0 || idx_x >=X_limit) continue;
            if(idx_y <0 || idx_y >=Y_limit) continue;
            if(i_cell[idx_x][idx_y] == ALIVE){ // alive
                count +=1;
            }
        }
    }
    return count;
}
