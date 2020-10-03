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
#define key_len 16
#define MAX_textlen 102401

int file_size;
typedef unsigned char uc;
typedef struct p_ham{
    unsigned char key[key_len];
    unsigned char p[36];
}store;
store* ham;
uc* base64_in;
//brute forece 1002_1832
void read_passwd(FILE* passwd_file);
void read_text(FILE* f,uc* plain,uc* cipher);
void into_base64(uc* in, int len);

int main(){
    // buffer for encrypt
    ham = calloc(line_num,sizeof(store));
    uc* plain,*cipher; 
    uc des[MAX_textlen], aes[MAX_textlen];
    plain = calloc(MAX_textlen, sizeof(uc));
    cipher = calloc(MAX_textlen+1, sizeof(uc));
    base64_in = calloc(MAX_textlen, sizeof(uc));

    //read password file
    FILE *passwd_file;
    passwd_file = fopen("passwords.txt","r");
    read_passwd(passwd_file);
    fclose(passwd_file);

    //read plaintextciphertext.txt
    FILE* input_file;
    input_file = fopen("PlaintextCiphertext.txt","r");
    read_text(input_file,plain,cipher);
    fclose(input_file);

    //DES encrypt
    int plain_len = strlen(plain)-1;
    int idx1,idx2; // key for idx
    idx1 = idx2 = 0;
    int iter = ((plain_len/16*16 == plain_len))?(plain_len):((plain_len/16+1)* 16);
    if(plain_len < iter){
        memset(plain+plain_len,0x0,iter-plain_len);
        plain[iter] ='\0';
    }  
    // elements for key setting
    uc key1[16],key2[16];
    DES_key_schedule keysched;
    AES_KEY enc_key_128;
	unsigned char iv[AES_BLOCK_SIZE];
    
    while(idx1<line_num){
        memcpy(key1,ham[idx1].key,key_len);
        DES_set_key((DES_cblock *)key1, &keysched);
        DES_set_key_checked((DES_cblock *)key1, &keysched);
        for(int i=0; i<iter; i+=8){
            DES_ecb_encrypt((DES_cblock *)(plain+i),(DES_cblock *)(des+i), &keysched, DES_ENCRYPT);
        }
        idx2=0;
        while(idx2<line_num){
            memset(iv, 0x00, AES_BLOCK_SIZE);
            memcpy(key2,ham[idx2].key,key_len);

            AES_set_encrypt_key(key2, 16*8, &enc_key_128);
            AES_cbc_encrypt(des,aes, strlen(des) , &enc_key_128, iv, AES_ENCRYPT);
            into_base64(aes,iter);

            //check whether ciphered plaintext vs ciphertext
            if(strncmp(base64_in,cipher,strlen(cipher)) == 0) {
                goto exit;
            } 
            idx2++;
        }
        idx1++;
    }
exit:
    free(plain);
    free(cipher);

    FILE* output = fopen("keys.txt","w");
    fprintf(output,"%s\n%s",ham[idx1].p,ham[idx2].p);
    fclose(output);

    free(base64_in);
    free(ham);
    return 0;
}


void into_base64(uc* in, int len){
    BIO *mem, *v_64;
    BUF_MEM *ptr;

    v_64 = BIO_new(BIO_f_base64());
    mem = BIO_new(BIO_s_mem());
    v_64 = BIO_push(v_64, mem);

    BIO_set_flags(v_64,BIO_FLAGS_BASE64_NO_NL);
    BIO_write(v_64,in,len);
    BIO_flush(v_64);
    BIO_get_mem_ptr(v_64,&ptr);

    BIO_set_close(v_64, BIO_NOCLOSE);
    memcpy(base64_in, ptr->data, ptr->length);
    base64_in[ptr->length]='\0';
    BIO_free_all(v_64);
    BUF_MEM_free(ptr); // free code needed

    return;
}
void read_text(FILE* f,uc* plain,uc* cipher){
    fgets(plain,MAX_textlen,f);
    fgets(cipher,MAX_textlen+1,f);
}

void read_passwd(FILE* passwd_file){
    char buf[BUFSIZE+4];
    unsigned char p[36];
    unsigned char hash[33];
    unsigned char key[key_len];
    int index=0;
    while(fgets(buf,sizeof(buf),passwd_file) !=NULL){
        //divide buf -> hash & p
        strcpy(p,(buf+33));
        p[strlen(p)-1] = '\0';
        strcpy(ham[index].p,p);
        
        buf[32]='\0';
        strcpy(hash,buf);
        //printf("hash : %s\n",hash);
        for(int j=0; j<key_len; ++j){
            int a[2];
            for(int k=0; k<2; ++k){
                int pos = 2*j +k;
                if(hash[pos] >= '0' && hash[pos] <= '9'){
                    a[k] = hash[pos]-'0';
                } else {
                    a[k] = hash[pos]-'a'+10;
                }
            }
            key[j] = a[0]<<4;
            key[j] += a[1];
        }

        memcpy(ham[index].key,key,key_len);
        index++;
    }
}