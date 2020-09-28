#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define SQUARE(x) ((x)*(x))

typedef struct twin{
    int max;
    int nu;
}twin;
char alpha[27] = {" etaonisrhlducmfwgpybvkxjqz"};
double percent[27] = {18.31, 12.60, 9.22, 8.01, 7.56, 7.00, 6.95, 6.52, 6.12, 
    6.11, 4.07, 4.03, 2.79, 2.74, 2.49, 2.43, 2.09, 1.99, 1.85, 1.75, 1.54, 
    0.98, 0.69, 0.17, 0.12, 0.10, 0.06};

int length_of_key;
int main(){
    unsigned char ch;
    double max =0;
    length_of_key=0;
    double tmp_sum =0;
    FILE *fpIn, *fpOut;
    char buf[5001];



    fpIn = fopen("hw1_input.txt","r");
    fseek(fpIn,0,SEEK_END);
    int buf_len = ftell(fpIn);
    fseek(fpIn,0,SEEK_SET);
    fread(buf,buf_len, sizeof(char), fpIn);
    fclose(fpIn);

    int count[260]={0, }; 
    memset(count,0,sizeof(int)*260);
    for(int i=1; i<=10; ++i){
        tmp_sum =0;

        memset(count,0,sizeof(int)*260);
        int j;
        for(j=1; i*j<buf_len; j++){
            int tmp = (int)buf[i*j];
            count[tmp]++;
        }

        for(int k=65; k<=90; ++k){
            tmp_sum += SQUARE((double)count[k])/SQUARE((double)j);
        }
        for(int k=97; k<=122; ++k){
            tmp_sum += SQUARE((double)count[k])/SQUARE((double)j);
        }
        if(max < tmp_sum){
                max = tmp_sum;
                length_of_key = i;
        }
    }

//=======================


    unsigned char key[length_of_key];
    memset(key,0,sizeof(unsigned char)*length_of_key);
FIND_KEY : 
    // find the key value
    for(int k=0; k<length_of_key; ++k){
        int counts[260];
        memset(counts,0,260*sizeof(int));
        // length_of_key * i + k
        for(int i=0; length_of_key*i+k<=buf_len; i++){
            int pos = length_of_key*i+k;
            counts[buf[pos]] = counts[buf[pos]]+1;
        }
        twin max_cnt,submax_cnt;   
        max_cnt.max =0;
        submax_cnt.max = 0;
        for(int i=0; i<=0xFF; ++i){
            if(max_cnt.max < counts[i]){
                submax_cnt.max = max_cnt.max;
                submax_cnt.nu = max_cnt.nu;
                max_cnt.max = counts[i];
                max_cnt.nu = i;
            } else if(submax_cnt.max < counts[i]){
                submax_cnt.max = counts[i];
                submax_cnt.nu = i;
            }
        }
        if(max_cnt.max*0.1 + submax_cnt.max > max_cnt.max){
            if(length_of_key *2 <= 10){
                length_of_key *=2;
                goto FIND_KEY;
            }
        }
        int ke = max_cnt.nu^' ';
        int ke1 = submax_cnt.nu^' ';
        key[k] = ke;
        if(ke != ke1){
            double prob1 = 0;
            double prob2 = 0;
            prob1 = percent[0]*max_cnt.max;
            prob2 = percent[0]*submax_cnt.max;
            for(int i=1; i<27; ++i){
                int diff = (int)(alpha[i]-' ');
                int pos1 = (max_cnt.nu + diff)%128;
                int pos2 = (submax_cnt.nu + diff)%128;
                //printf("%d %d\n",pos1,pos2);
                prob1 += percent[i] * counts[pos1];
                prob2 += percent[i] * counts[pos2];
            }
            if(prob2 > prob1){
                key[k] = ke1;
            } 
        }
    }
    // periodic check
    for(int i=1; i<length_of_key; ++i){
        int same=0;
        if(i == 1){
            for(int j=0; j<length_of_key; j++){
                if(key[j] == key[j+1]) same++;
                else break;
            }
            if(same == (length_of_key-1)){
                length_of_key = 1;
                break; // exit the period test
            } else same =0;
        }else if(length_of_key%i == 0){
            int period = length_of_key/i;
            int okay =0;
            if(key[0] == key[i]){
                //period 4 -> need to check 3 times
                for(int iter=0; iter<(period-1); ++iter){
                    same=0;
                    for(int j=0; j<i; ++j){
                        if(key[iter*i+j] == key[(iter+1)*i+j]) same++;
                        else break;
                    }
                    if(same == i) okay++;
                    else break;
                }
                if((okay+1) == period) length_of_key = i;
            }

        } // end else if(length_of_key%i == 0){
    } // end for(int i=1; i<length_of_key; ++i){


    fpOut = fopen("hw1_output.txt","wb");
    // for file write
    fprintf(fpOut,"%#x",key[0]);
    for(int i=1; i<length_of_key; ++i){
        fprintf(fpOut," %#.2x",key[i]);
    }
    fprintf(fpOut, "\n");
    for(int i=0; i<buf_len; ++i){
        ch = buf[i]^key[i%length_of_key];
        fwrite(&ch, sizeof(ch), 1, fpOut);
    }
    fclose(fpOut);
    return 0;
}