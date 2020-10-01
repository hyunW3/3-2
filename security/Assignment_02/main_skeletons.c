#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

#define line_num 184389
#define BUFSIZE 64
int file_size;
typedef unsigned char uc;
typedef struct p_ham{
    unsigned char key[16];
    unsigned char p[36];
}store;
store* ham;

void read_passwd(FILE* passwd_file);
void read_filesize(FILE* f);
void read_text(FILE* f,uc* plain,uc* cipher);

int main(){
    // buffer for encrypt
    ham = calloc(line_num,sizeof(store));
    uc* plain, *des, *aes, *cipher;
    uc* base64_in;
    plain = calloc(BUFSIZE, sizeof(uc));
    cipher = calloc(BUFSIZE, sizeof(uc));
    des = calloc(BUFSIZE, sizeof(uc));
    aes = calloc(BUFSIZE, sizeof(uc));
    base64_in = calloc(BUFSIZE, sizeof(uc));

    //read password file
    FILE *passwd_file;
    passwd_file = fopen("passwords.txt","r");
    read_filesize(passwd_file);
    read_passwd(passwd_file);
    fclose(passwd_file);

    //read plaintextciphertext.txt
    FILE* input_file;
    input_file = fopen("PlaintextCiphertext.txt","r");
    read_text(input_file,plain,cipher);
    fclose(input_file);

    free(base64_in);
    free(ham);
    return 0;
}

// need to receive whole text
void read_text(FILE* f,uc* plain,uc* cipher){
    fgets(plain,BUFSIZE,f);
    fgets(cipher,BUFSIZE,f);
    //printf("plain :%s\ncipher :[%s]\n",plain,cipher);
}
void read_filesize(FILE* f){
    fseek(f,0,SEEK_END);
    file_size = ftell(f);
    fseek(f,0,SEEK_SET);
    printf("file_size : %d\n",file_size);

}
void read_passwd(FILE* passwd_file){
    char buf[BUFSIZE+4];
    unsigned char p[36];
    unsigned char hash[33];
    unsigned char key[16];
    int index=0;
    while(fgets(buf,sizeof(buf),passwd_file) !=NULL){
        //printf("%s\n",buf);
        //divide buf -> hash & p
        strcpy(p,(buf+33));
        p[strlen(p)-1] = '\0';
        strcpy(ham[index].p,p);
        
        buf[32]='\0';
        strcpy(hash,buf);
        //printf("hash : %s\n",hash);
        for(int j=0; j<16; ++j){
            int a[2];
            for(int k=0; k<2; ++k){
                int pos = 2*j +k;
                if(hash[pos] >= '0' && hash[pos] <= '9'){
                    a[k] = hash[pos]-'0';
                } else {
                    a[k] = hash[pos]-'a'+10;
                }
            }
            //printf("%01x %01x\n",a[0],a[1]);
            key[j] = a[0]<<4; // + a[1];
            key[j] += a[1];
        }

        memcpy(ham[index].key,key,16);
    /*
        if(index<3) {
            for(int j=0; j<16; ++j){
                printf("%02x ",ham[index].key[j]);
            }
            printf("\n");
            printf("%s\n",ham[index].p);
        }
        //if(index >=1) break;
    */
        index++;
        // line 29131 -> bufsize should be longer than 65
    }
    printf("line is %d\n",index);
}