#include <stdio.h>
#include <iostream>
#include <cstring>
int X_limit = 1024;
int Y_limit = 1024;
int main(){
    for(int i=0; i<10; i++){
        printf("i is %d\n",i);
        char alive_map[X_limit*Y_limit*i]={0,};
        bool i_cell[X_limit*Y_limit*i]={0,}; // 0~X_limit-1 * 0~Y_limit-1
        bool tmp_i_cell[X_limit*Y_limit*i]={0,};
        memset(alive_map,0,X_limit*Y_limit*sizeof(char)*i);
        memset(i_cell,0,X_limit*Y_limit*sizeof(bool)*i);
        memset(tmp_i_cell,0,X_limit*Y_limit*sizeof(bool)*i);
    }
}