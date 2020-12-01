#include <stdio.h>
#include <iostream>
#include <cstring>
#include <mpi.h>
#include <math.h>
#define ALIVE 1
#define DEAD 0
int X_limit = 1024;
int Y_limit = 1024;
int myrank[2] = {0,0};
int x_range = 12;
int y_range = 18;
    int x_low_bound = (myrank[0]*x_range);
    int x_upper_bound =  (myrank[0]+1)*x_range;
    int y_low_bound = (myrank[1]*y_range);
    int y_upper_bound = (myrank[1]+1)*y_range;
__inline 
bool check_bound(int row, int col){
    return ( x_low_bound <= row) && (row < x_upper_bound) && (y_low_bound <= col) && (col < y_upper_bound );
}
int main(int argc, char *argv[]){
    std::cout << check_bound(6,8);
    std::cout << check_bound(13,8);
}