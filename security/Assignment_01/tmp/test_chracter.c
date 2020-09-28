#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define SQUARE(x) ((x)*(x))

int main(){

    for(int i=0; i<20; i++){
        printf("%d - %d\n",i,SQUARE(i));
    }
    
}