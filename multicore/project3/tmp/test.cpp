#include <stdio.h>
#include <iostream>
#include <cstring>
#define ALIVE 1
#define DEAD 0
int X_limit = 1024;
int Y_limit = 1024;
int main(){
    char**  alive_map = new char*[X_limit];
    char**  i_cell = new char*[X_limit];
    char**  tmp_i_cell = new char*[X_limit];
    for(int i=0; i<X_limit; i++){
        alive_map[i] = new char[Y_limit];
        memset(alive_map[i],0,Y_limit*sizeof(char));
        i_cell[i] = new char[Y_limit];
        memset(i_cell[i],0,Y_limit*sizeof(char));
        tmp_i_cell[i] = new char[Y_limit];
        memset(tmp_i_cell[i],0,Y_limit*sizeof(char));
    }
    /*
    memset(alive_map,0,X_limit*Y_limit*sizeof(char));
    memset(i_cell,0,X_limit*Y_limit*sizeof(char));
    memset(tmp_i_cell,0,X_limit*Y_limit*sizeof(char));
    */
    for(int i=0; i<70; i++){
        i_cell[i][i] = 1;
    }
    for(int i=0; i<70; i++){
        for(int j=0; j<70; j++){
            if(i_cell[i][j] != 0) printf("%d %d : %d\n",i,j,i_cell[i][j]);
        }
    }
}