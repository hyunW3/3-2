#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
// 2nd code for 0915
typedef struct twin{
    int max;
    int nu;
}twin;
char alpha[27] = {" etaonisrhlducmfwgpybvkxjqz"};
double percent[27] = {18.31, 10.21, 7.51, 6.55, 6.2, 5.7, 5.7, 5.33, 4.97, 4.86, 3.36, 3.35, 2.30, 2.27, 2.02, 1.97, 1.69, 1.64, 1.5, 1.47, 1.27, 0.79, 0.57, 0.15, 0.11, 0.09, 0.06};

int length_of_key;
int main(){
    unsigned char ch;
    unsigned int len;
    FILE *fpIn, *fpOut;
    char buf[5001];
    fpIn = fopen("hw1_input.txt","r");
    fseek(fpIn,0,SEEK_END);
    int buf_len = ftell(fpIn);
    fseek(fpIn,0,SEEK_SET);
    fread(buf,buf_len, sizeof(char), fpIn);
    printf("%d\n",buf_len);
    fclose(fpIn);
    //printf("%c %c %c\n",buf[0],buf[buf_len-1],buf[buf_len]);
    int count[260]={0, }; 
    memset(count,0,sizeof(int)*260);
    double max =0;
    int length_of_key=0;
    double tmp_rms = 0;
    double tmp_sum =0;
    for(int i=1; i<=10; ++i){
        tmp_rms = 0;
        tmp_sum =0;
        memset(count,0,sizeof(int)*260);
        int j;
        for(j=1; i*j<buf_len; j++){
            int tmp = (int)buf[i*j];
            count[tmp]++;
        }
        /*
        for(int k=0; k<256; ++k){
            tmp_sum += pow(count[k],2)/pow(j,2);
        }
        */
        for(int k=97; k<=122; ++k){
            count[k-32] += count[k];
        }
        for(int k=65; k<=90; ++k){
            tmp_sum += pow(count[k],2)/pow(j,2);
        }

        //tmp_rms = sqrt(tmp_sum/j);
        tmp_rms = sqrt(tmp_sum);
        printf("%.2dth tmp_rms : %0.5lf\n",i,tmp_rms);
        if(max < tmp_rms){
            if(max+0.0003 > tmp_rms) {
                printf("skpped max : %dth\n",i);
                continue;
            } else{
                max = tmp_rms;
                length_of_key = i;
                printf("max : %dth\n",i);
            }
        }
        // need to be diff b/w max & submax
    }
    printf("%d %lf\n",length_of_key,max);

//=======================

    unsigned char key[length_of_key];
    memset(key,0,sizeof(int)*length_of_key);
    // find the key value
    for(int k=0; k<length_of_key; ++k){
        printf("%d -",k);
        int counts[260];
        memset(counts,0,260*sizeof(int));
        // length_of_key * i + k
        for(int i=0; length_of_key*i+k<=buf_len; i++){
            int pos = length_of_key*i+k;
            counts[buf[pos]] = counts[buf[pos]]+1;
            //printf("%d %d\n",buf[pos], counts[buf[pos]]);
        }
        //shift to Capital to lower
        for(int i=0; i<26; i++){
            
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
        printf("max :%d(%dth), submax : %d(%dth)\n' '(space) : %d\n",max_cnt.max,
                        max_cnt.nu,submax_cnt.max,submax_cnt.nu,' ');
        int ke = max_cnt.nu^' ';
        int ke1 = submax_cnt.nu^' ';
        key[k] = ke;
        if(ke != ke1){
            double prob1,prob2;
            prob1 = 18.31*max_cnt.max;
            prob2 = 18.31*submax_cnt.max;
            for(int i=1; i<27; ++i){
                int diff = (int)(alpha[i]-' ');
                prob1 += percent[i] * counts[max_cnt.nu + diff] / 10;
                prob2 += percent[i] * counts[submax_cnt.nu + diff] / 10;
            }
            printf("%lf vs %lf\n",prob1,prob2);
            if(prob2 > prob1){
                key[k] = ke1;
                printf("ke1 : %x\n",ke1);
            } else  printf("ke : %x\n",ke);
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
            printf("period : %d\n",i);
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
                //printf("%d %d %d\n",okay,period,same);
            }

        } // end else if(length_of_key%i == 0){
    } // end for(int i=1; i<length_of_key; ++i){
    printf("key length : %d\n",length_of_key);
    printf("key is\n");
    for(int i=0; i<length_of_key; ++i){
        printf("%#.2x ",key[i]);
    }    
    printf("\n");

    fpOut = fopen("hw1_output.txt","wb");
    // for file write
    fprintf(fpOut,"%#x",key[0]);
    for(int i=1; i<length_of_key; ++i){
        fprintf(fpOut," %#.2x",key[i]);
    }
    fprintf(fpOut, "\n");
    //printf("%s\n",buf);
    for(int i=0; i<buf_len; ++i){
        ch = buf[i]^key[i%length_of_key];
        fwrite(&ch, sizeof(ch), 1, fpOut);
    }
    
    fclose(fpOut);
}