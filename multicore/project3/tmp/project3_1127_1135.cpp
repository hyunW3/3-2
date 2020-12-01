#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <string.h>
#define ALIVE true
#define DEAD false

int X_limit,Y_limit;
char get_num_alive(bool* i_cell,int x_pos,int y_pos);
int main(int argc, char *argv[]){
    int size,rank,num_Gen;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

///  get argument
    if(argc != 5){
        perror("exit:wrong parameter number\n");
        MPI_Finalize();
        return 1;
    }
    num_Gen = atoi(argv[2]);
    X_limit = atoi(argv[3]);
    Y_limit = atoi(argv[4]);

    char* alive_map = new char[X_limit*Y_limit];
    bool* i_cell = new bool[X_limit*Y_limit];
    bool* tmp_i_cell = new bool[X_limit*Y_limit];
    memset(alive_map,0,X_limit*Y_limit*sizeof(char));
    memset(i_cell,0,X_limit*Y_limit*sizeof(bool));
    memset(tmp_i_cell,0,X_limit*Y_limit*sizeof(bool));
///  get input
    //FILE* f = fopen(filename,"r");
    FILE* f = fopen(argv[1],"r");
    if (f == NULL) perror("Error in opening file\n");
    
    int row,col;
    while (fscanf(f,"%d %d\n",&row,&col) != EOF) {
        //printf("%d %d\n",row,col);
        i_cell[row*Y_limit+col] =ALIVE;
    }
    fclose(f);
    
/// parallel version 
    //for num of Gen
    for(int g=0; g<num_Gen; g++){
        for(int i=0; i<X_limit; i++){
	        for(int j=0; j<Y_limit; j++){ // i,j
                // get pos and cal alive or dead neigherhood
                int index = i*Y_limit+j;
                if((index)%size == rank){
                    alive_map[index] = get_num_alive(i_cell,i,j);
                    // local_alive_map
                }
            }
        }
        for(int i=0; i<X_limit; i++){
            for(int j=0; j<Y_limit; j++){
                int index = i*Y_limit+j;
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
        if (g != num_Gen-1){
            MPI_Allreduce(tmp_i_cell,i_cell,X_limit*Y_limit,MPI_C_BOOL,MPI_LOR,MPI_COMM_WORLD);
        } else {
            // print cell location
            for(int i=0; i<X_limit; i++){
                for(int j=0; j<Y_limit; j++){
                    if(tmp_i_cell[i*Y_limit+j] == ALIVE){
                        printf("%d %d\n",i,j);
                    }
                }
            }
        }
    } //end for(int g=0; g<num_Gen)
/// finish program
    
    MPI_Finalize();
    return 0;
}

__inline 
char get_num_alive(bool* i_cell,int x_pos,int y_pos){
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
            if(i_cell[idx_x*Y_limit+idx_y]==ALIVE){ // alive
                count +=1;
            }
        }
    }
    return count;
}
