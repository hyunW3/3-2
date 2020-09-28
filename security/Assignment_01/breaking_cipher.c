#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LENGTH 10
// for test, the key length 
// will be less than or equal 10bytes
// https://www.youtube.com/watch?v=LaWp_Kq0cKs&t=494s
//double probability[11]= {0,}; // length 1~10 
// 1st code
typedef struct twin{
    int max;
    int nu;
}twin;
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
    //fgets(buf, sizeof(buf),fpIn)
    printf("%d\n",buf_len);
    int count[11]; 
    twin top;
    twin subtop;
    subtop.max = -1;
    //
    for(int key_len=1; key_len<=MAX_LENGTH; ++key_len){
        //printf("%d\n",key_len);
        //int count[260]={0,}; 
        count[key_len] = 0;
        for(int i=0; i<buf_len; i++){
            if(i<buf_len-10){
                if(buf[i] == buf[i+key_len]) count[key_len]++;
            }else {
                if(buf[i] == buf[(i+key_len)%buf_len]) count[key_len]++;
            }
        }
        // determine max
        if(key_len == 1){
            top.max = count[key_len];
            subtop.max = count[key_len];
            top.nu = key_len;
        }else {
            if(count[key_len] > top.max){
                subtop.max = top.max;
                subtop.nu = top.nu;
                top.max = count[key_len];
                top.nu = key_len;
            } else if(count[key_len] > subtop.max){
                subtop.max = count[key_len];
                subtop.nu = key_len;
            }
        }
        //printf("%d %d\n",subtop.max,top.max);
        // subtop problem 0912 14:43
    }
    for(int i=1; i<=MAX_LENGTH; ++i){
        printf("%d : %d\n",i,count[i]);
    }
    printf("==============%d %d\n",subtop.max,top.max);
    // deternmine KEY LENTH 
    int small = (top.nu > subtop.nu)?(subtop.nu):(top.nu);
    int big = (top.nu <= subtop.nu)?(subtop.nu):(top.nu);
    printf("big : %d, small : %d\n",big,small);
    if(subtop.max*2+1 < top.max){
        length_of_key = top.nu;
    }else if(big%small == 0){
        length_of_key = small;
    }else {
        length_of_key = big-small;
        printf("candidate : %d(=small)\n",small);
        // 후보 small도 가능하지않을까
    }
    printf("length of key : %d\n",length_of_key);
    // finish to find key of length

    // ======
    
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
            int prob1 = 18*max_cnt.max + 10*counts[(max_cnt.nu+(int)('e'-' '))] + 7.5*counts[(max_cnt.nu+(int)('t'-' '))]; // 18% for space, 10% for 'e'
            int prob2 = 18*submax_cnt.max + 10*counts[(submax_cnt.nu+(int)('e'-' '))] + 7.5*counts[(submax_cnt.nu+(int)('t'-' '))]; // 18% for space, 10% for 'e'
            printf("%d == %d vs %d\n",prob1,prob2);
            if(prob2 > prob1){
                key[k] = ke1;
                printf("ke1 : %d\n",ke1);
            } else  printf("ke : %d\n",ke);
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
    fclose(fpIn);
    fclose(fpOut);
}

/*
    while(1){
        if(fgets(buf, sizeof(buf),fpIn) == NULL) break;
    }
    printf("%s",buf);
*/