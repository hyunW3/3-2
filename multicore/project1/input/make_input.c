#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char op[4] = {'i','w','p','q'};
int main(int argc, char* argv[]){
    //char* name = argv[1];
    srand(time(NULL));
    int line = rand() % 1000;
    FILE* input = fopen("./input/input_1.txt","w");
    for(int i=0; i<line; ++i){
        //srand(time(NULL));
        char oper = op[rand()%4];
        //srand(time(NULL));
        if(oper != 'p') {
            int num = rand() % 1000 + 1;
            fprintf(input,"%c %d\n",oper,num);
        } else fprintf(input,"%c\n",'p');
    }
    fclose(input);
}