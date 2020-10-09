#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char op[4] = {'i','w','q','p'};
int main(int argc, char* argv[]){
    //char* name = argv[1];
    srand(time(NULL));
    int line = rand() % 110000+500000;
    //srand(time(NULL));
    int p_line = rand() %600 + 100;
    int k =1;
    FILE* input = fopen("./input/input_5.txt","w");
    for(int i=0; i<line; ++i){
        //srand(time(NULL));
        char oper;
        if(i == p_line*k) {
            oper = 'p';
            k++;
        }else  oper = op[rand()%3];
        //srand(time(NULL));
        if(oper != 'p') {
            int num = rand() % 10000 + 1;
            fprintf(input,"%c %d\n",oper,num);
        } else fprintf(input,"%c\n",'p');
    }
    fprintf(input,"%c\n",'p');
    fclose(input);
}