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
#include <time.h>

#define line_num 184389
#define BUFSIZE 64
#define key_len 16
int file_size;
typedef unsigned char uc;
typedef struct p_ham{
    unsigned char key[key_len];
    unsigned char p[36];
}store;
store* ham;

void read_passwd(FILE* passwd_file);
void read_filesize(FILE* f);
void read_text(FILE* f,uc* plain,uc* cipher);
uc* into_base64(uc* in, int len);

int main(){
    // buffer for encrypt
    ham = calloc(line_num,sizeof(store));
    uc* plain,*cipher; //*des, *aes, ;
    uc des[BUFSIZE], aes[BUFSIZE];
    uc* base64_in;
    plain = calloc(BUFSIZE, sizeof(uc));
    cipher = calloc(BUFSIZE+1, sizeof(uc));
    //des = calloc(BUFSIZE, sizeof(uc));
    //aes = calloc(BUFSIZE, sizeof(uc));
    base64_in = calloc(BUFSIZE, sizeof(uc));

    //read password file
    FILE *passwd_file;
    passwd_file = fopen("passwords.txt","r");
    read_filesize(passwd_file);
    read_passwd(passwd_file);
    fclose(passwd_file);

    //read plaintextciphertext.txt
    FILE* input_file;
    input_file = fopen("encryption_input.txt","r");
    read_text(input_file,plain,cipher);
    fclose(input_file);

    //DES encrypt
    int plain_len = strlen(plain)-1;
    printf("plain_len : %d\n",plain_len);
    int idx1,idx2; // key for idx
    idx1 = idx2 = 0;
    int iter = ((plain_len/16*16 == plain_len))?(plain_len):((plain_len/16+1)* 16);
    printf("iter : %d\n",iter);
    if(plain_len < iter) memset(plain+plain_len,0x0,iter-plain_len);
    
    // elements for key setting
    //uc *key1,*key2;
    uc key1[16],key2[16];
    DES_key_schedule keysched;
    AES_KEY enc_key_128;
	unsigned char iv[AES_BLOCK_SIZE];
    //key1 = calloc(key_len,sizeof(uc)); // key1 for DES
    //key2 = calloc(key_len,sizeof(uc)); // key for AES
/*
    srand(time(NULL));
    idx1= rand() % 400;
    srand(time(NULL));
    idx2 = rand() % line_num;
    */
   idx1 = 145;
   idx2 = 180024;
    printf("==========idx : %d %d==========\n",idx1,idx2);
    printf("====password : %s,%s==========\n",ham[idx1].p,ham[idx2].p);
    
    FILE* key_file = fopen("key_file.txt","w");
    fprintf(key_file,"%d-%s\n%d-%s\n",idx1,ham[idx1].p,idx2,ham[idx2].p);
    fclose(key_file);

           memcpy(key1,ham[idx1].key,key_len);
        DES_set_key((DES_cblock *)key1, &keysched);
        DES_set_key_checked((DES_cblock *)key1, &keysched);
        for(int i=0; i<iter; i+=8){
            DES_ecb_encrypt((DES_cblock *)(plain+i),(DES_cblock *)(des+i), &keysched, DES_ENCRYPT);
        }
        //base64_in = into_base64(des,iter);
            //printf("des result:\n%s\n",base64_in);
        
        memset(iv, 0x00, AES_BLOCK_SIZE);
        memcpy(key2,ham[idx2].key,key_len);

        AES_set_encrypt_key(key2, 16*8, &enc_key_128);
        printf("%ld %ld\n",strlen(des),sizeof(des));
        AES_cbc_encrypt(des,aes, sizeof(des) , &enc_key_128, iv, AES_ENCRYPT);
        base64_in = into_base64(aes,iter);

        printf("result:\n%s\n",base64_in);
        FILE* out = fopen("PlaintextCiphertext.txt","w");
        fprintf(out,"%s",plain);
        fprintf(out,"%s",base64_in);
        fclose(out);
            //check whether ciphered plaintext vs ciphertext
 
    fclose(out);
exit:

    //after write the key 
    //free(key1);
    //free(key2);


    free(base64_in);
    free(ham);
    return 0;
}

// need to receive whole text
uc* into_base64(uc* in, int len){
    BIO *bio_mem, *bio_64;
    BUF_MEM *bio_ptr;

    bio_64 = BIO_new(BIO_f_base64());
    bio_mem = BIO_new(BIO_s_mem());
    bio_64 = BIO_push(bio_64, bio_mem);

    BIO_write(bio_64,in,len);
    BIO_flush(bio_64);
    BIO_get_mem_ptr(bio_64,&bio_ptr);

    char* out = calloc(bio_ptr->length,sizeof(uc));
    memcpy(out, bio_ptr->data, bio_ptr->length-1);
    out[bio_ptr->length-1] = 0;
    BIO_free_all(bio_64);

    return out;
}
void read_text(FILE* f,uc* plain,uc* cipher){
    fgets(plain,BUFSIZE,f);
    printf("plain :%s",plain);
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
    unsigned char key[key_len];
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
            //printf("%01x %01x\n",a[0],a[1]);
            key[j] = a[0]<<4; // + a[1];
            key[j] += a[1];
        }

        memcpy(ham[index].key,key,key_len);
    /*
        if(index<3) {
            for(int j=0; j<key_len; ++j){
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