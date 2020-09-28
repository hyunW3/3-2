#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
// 2nd code for 0915

int length_of_key;
int main(){
    unsigned char ch;
    unsigned int len;
    FILE *fpIn, *fpOut;
    char buf[5001];
    fpIn = fopen("hw1_input.txt","r");
    fpOut = fopen("hw1_output.txt","wb");
    fseek(fpIn,0,SEEK_END);
    int buf_len = ftell(fpIn);
    fseek(fpIn,0,SEEK_SET);
    fread(buf,buf_len, sizeof(char), fpIn);
    printf("%d\n",buf_len);

    int count[260]={0, }; 
    memset(count,0,sizeof(int)*260);
    double max =0;
    int length=0;
    double tmp_rms = 0;
    double tmp_sum =0;
    for(int i=3; i<=3; ++i){
        tmp_rms = 0;
        tmp_sum =0;
        double list_rms[i];
        memset(count,0,sizeof(int)*260);
        memset(list_rms,0,sizeof(double)*i);
        for(int r=1; r<i; ++r){
            int iter=0;
            for(iter; iter*i+r<buf_len; ++iter){
                printf("%d %d\n",iter,iter*i+r);
                int tmp = (int)buf[iter*i+r];
                count[tmp]++;
            }
            for(int j=0; j<256; ++j){
                tmp_sum += pow(count[j],2);
            }
            list_rms[r] = sqrt(tmp_sum)/(iter);
            printf("%dth rms : %0.2lf\n",r,list_rms[r]);
        }
        for(int r=0; r<i; ++r){
            tmp_rms += list_rms[r];
        }
        tmp_rms /= i;
        printf("%.2dth tmp_rms : %0.2lf\n",i,tmp_rms);
        if(max < tmp_rms){
            max = tmp_rms;
            length = i;
        }
    }
    printf("%d %lf\n",length,max);

}