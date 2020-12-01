#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>
#define ALIVE 1
#define DEAD 0

int X_limit,Y_limit;
char get_num_alive(int x_pos,int y_pos);
char** i_cell;

int x_low_bound;
int x_upper_bound;
int y_low_bound;
int y_upper_bound;
__inline 
bool check_bound(int row, int col){
    return ( x_low_bound <= row) && (row < x_upper_bound) && (y_low_bound <= col) && (col < y_upper_bound );
}
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
    int axis_p_num =(int)sqrt(size);
    int x_range = (int)(X_limit/axis_p_num);
    int y_range = (int)(Y_limit/axis_p_num);
    MPI_Comm comm_2d;
    int dims[2] = {axis_p_num,axis_p_num};
    int periods[2] = {1,1};
    MPI_Cart_create(MPI_COMM_WORLD, 2,dims,periods,1,&comm_2d );
    int my2D_rank;
    MPI_Comm_rank(comm_2d,&my2D_rank);
    int myrank[2];
    MPI_Cart_coords(comm_2d,my2D_rank ,2, myrank);

    MPI_Status *status = new MPI_Status *[4];

    char** alive_map = new char*[x_range]; // up & down
    i_cell = new char*[x_range];
    char** x_i_cell = new char*[2];
    char** y_i_cell = new char*[2];

    for(int i=0; i<x_range; i++){
        alive_map[i] = new char[y_range];
        memset(alive_map[i],0,y_range*sizeof(char));
        i_cell[i] = new char[y_range];
        memset(i_cell[i],0,y_range*sizeof(char));
        x_i_cell[i] = new char[y_range];    // up & down
        memset(x_i_cell[i],0,y_range*sizeof(char));
        y_i_cell[i] = new char[2];    // left & right
        memset(xy_i_cell[i],0,2*sizeof(char));
    }
//    printf("rank(%d) - %s %d %d %d\n",rank,argv[1],num_Gen,X_limit,Y_limit);
///  get input
    //FILE* f = fopen(filename,"r");
    FILE* f = fopen(argv[1],"r");
    if (f == NULL) perror("Error in opening file\n");
    
    int row,col;
    x_low_bound = (myrank[0]*x_range);
    x_upper_bound =  (myrank[0]+1)*x_range;
    y_low_bound = (myrank[1]*y_range);
    y_upper_bound = (myrank[1]+1)*y_range;
    while (fscanf(f,"%d %d\n",&row,&col) != EOF) {
        //printf("%d %d\n",row,col);
        if( check_bound(row,col) ) {
            i_cell[row -(myrank[0]*x_range)][col-(myrank[1]*y_range)] = ALIVE;
        }
        //if(i_cell[row][col] == ALIVE) printf("%d %d - %d\n",row,col,i_cell[row][col]);
    }
    fclose(f);
    
/// for test whether input get well
/*
    if (myrank[1] == 1 && myrank[0] == 0){ // for test whether input get well
        for(int i=0; i<x_range; i++){
	        for(int j=0; j<y_range; j++){ // i,j
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
        for(int i=0; i<x_range; i++){
	        for(int j=0; j<y_range; j++){ // i,j
                // get pos and cal alive or dead neigherhood
                alive_map[i][j] = get_num_alive(i,j);
                    //if(alive_map[i][j] != 0 ) printf("%d %d %d",i,j,alive_map[i][j]);
                    // local_alive_map
            }
        }
        for(int i=0; i<x_range; i++){
            for(int j=0; j<y_range; j++){
                if((i_cell[i][j] ==ALIVE) && alive_map[i][j]  ==2){
                    i_cell[i][j] = ALIVE;
                } else if(alive_map[i][j] ==3){
                    i_cell[i][j]  = ALIVE;
                } else {    
                    i_cell[i][j]  = DEAD;
                }
            }
        }
        if (g == num_Gen-1){
            // print cell location
            for(int i=0; i<x_range; i++){
                for(int j=0; j<y_range; j++){
                    if(tmp_i_cell[i][j]  == ALIVE){
                        printf("%d %d\n",i,j);
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
    

	local_finish = MPI_Wtime();
	local_elapsed = local_finish - local_start;
	MPI_Reduce(&local_elapsed,&elapsed,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);


	//if(!rank)   printf("Elapsed time    : %.3f seconds\n\n",elapsed);

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
