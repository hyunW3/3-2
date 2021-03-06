#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <string.h>

char** cell; // 0~X_limit-1 * 0~Y_limit-1
int X_limit,Y_limit;
int** alive_map;
int get_num_alive(int x_pos,int y_pos);
int main(int argc, char *argv[]){
    int size,rank,num_Gen;
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
    cell = (char**)calloc(Y_limit,sizeof(char*));
    alive_map = (int**)calloc(Y_limit,sizeof(int*));
    for(int i=0; i<Y_limit; i++){
        cell[i] = (char*)calloc(X_limit+2,sizeof(char));
        alive_map[i] = (int*)calloc(X_limit,sizeof(int));
    }
//    printf("rank(%d) - %s %d %d %d\n",rank,filename,num_Gen,X_limit,Y_limit);
///  get input
    FILE* f = fopen(filename,"r");
    if (f == NULL) perror("Error in opening file\n");
    
    for(int y=0; y<Y_limit; y++){
        fgets(cell[y],X_limit+2,f);
        cell[y][X_limit] = '\0';
    }
/* for test whether input get well
    if (rank == 0){ // for test whether input get well
        for(int y=0; y<Y_limit; y++){
            printf("%d-%s\n",y,cell[y]); // for test whether input get well
        }
    }
*/
    fclose(f);
/// serial version 
    //for num of Gen
    if(rank == 0){
    for(int g=0; g<num_Gen; g++){
        for(int i=0; i<Y_limit; i++){
            for(int j=0; j<X_limit; j++){
                // get pos and cal alive or dead neigherhood
                //int n_alive = get_num_alive(i,j);
                alive_map[i][j] = get_num_alive(i,j);
                //printf("%d",n_alive);
            }
            //printf("\n");
        }
        for(int i=0; i<Y_limit; i++){
            for(int j=0; j<X_limit; j++){
                if(cell[i][j] == '#'){ // alive
                    if((alive_map[i][j] <2 )||(alive_map[i][j] >3)){
                        // next gen dead 0~1 and 4~8 case when cell alive
                        cell[i][j] = '.';
                    } else cell[i][j] ='#';
                } else { // dead
                    if(alive_map[i][j] != 3){ 
                        // next gen dead
                        cell[i][j] = '.';
                    } else cell[i][j] ='#';
                }
                printf("%c",cell[i][j]);
            }
            printf("\n");
        }
        printf("==================\n");
    }
    } // end if (rank ==0)
/// finish program
    free(filename);
    for(int i=0; i<Y_limit; i++){
        free(cell[i]);
    }
    free(cell);
    
    MPI_Finalize();
    return 0;
}

int get_num_alive(int x_pos,int y_pos){
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
            if(cell[idx_x][idx_y]=='#'){ // alive
                count +=1;
            }
            /*
            if(x_pos == 1 && y_pos == 4){
                printf("%d %d - %d %c\n",idx_x,idx_y,count,cell[idx_x][idx_y]);
            }
            */
        }
    }
    return count;
}