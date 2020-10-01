#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/des.h>

#define max_line 184389 // lines # of password
#define BUF_SIZE 64
//static unsigned char* aes_key;
//static unsigned char* des_key;

int main(){
    FILE* passwd_file = fopen("passwords.txt","r");
    FILE* input_file;//,output_file;
    unsigned char p1[128], p2[128];
    unsigned char input[BUF_SIZE], tmp[BUF_SIZE], output[BUF_SIZE];
	memset(input, 0, BUF_SIZE);
	memset(output, 0, BUF_SIZE);
	memset(tmp, 0, BUF_SIZE);
    int seq=1;
    	/* Init vector */
	unsigned char iv[AES_BLOCK_SIZE];
	memset(iv, 0x00, AES_BLOCK_SIZE);
    
    int i=0;
    
    while(i<=max_line){
        if(seq == 1) fgets(p1,sizeof(p1),passwd_file);
        else if(seq == 2){
            fgets(p2,sizeof(p2),passwd_file);
            break;
        } 
        seq++;
        i++;
    } 
    fclose(passwd_file);
/*
    printf("%s",p1);
    printf("%s",p2);
    memset(p1,0,sizeof(p1));
    memset(p2,0,sizeof(p2));
*/
    // use strtok
    p1[16]='\0';
    p2[32]='\0'; 
    printf("%s\n",p1);
    printf("%s\n",p2);
    unsigned char p11[16];
    strcpy(p11,p1);
    unsigned char p22[32];
    strcpy(p22,p2);

    input_file = fopen("PlaintextCiphertext.txt","r");
    fgets(input,BUF_SIZE,input_file);
    printf("%s",input);
    DES_key_schedule D_keysched;
    int ret = DES_set_key((DES_cblock*)p11, &D_keysched);
    if(ret <0) printf("DES set key failed\n");
    ret = DES_ecb_encrypt((DES_cblock*)input,(DES_cblock*)tmp, &D_keysched,DES_ENCRYPT);
    if(ret <0) printf("DES cbc encrypt failed\n");
    printf("======tmp is\n%s\n",tmp);
    unsigned char* e = tmp;
    while(*e) printf(" [%02x]", *e++);
    printf("\n");
    AES_KEY enc_key;
     
    AES_set_encrypt_key(p22,16*8,&enc_key);    
    AES_cbc_encrypt(tmp, output, sizeof(tmp), &enc_key, iv, AES_ENCRYPT);

    memset(iv, 0x00, AES_BLOCK_SIZE);
   // printf("%s\n",output);
    e = output;
    
    while(*e) printf("%c", *e++); //printf(" [%02x]", *e++);
    printf("\n");
    return 0;
}