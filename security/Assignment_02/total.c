#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

#define BUFSIZE 64
char *base64(const unsigned char *input, int length);
void print_out_hexa(unsigned char* e);
void print_out_char(unsigned char* e);
int main(){

    unsigned char in[BUFSIZE], out[BUFSIZE], back[BUFSIZE];
    unsigned char *e = out;
    FILE* out = fopen("PlaintextCiphertext.txt","w");
    DES_cblock key;
    DES_key_schedule keysched;
    char clear[] = "SKKU is the top university in the world";
    int clear_len = sizeof(clear)-1;
    
    unsigned char origin_key[] = {0x00,0x00,0x99,0x65,0x6c,0x16,0xf8,0xe8,
                            0x4f,0x50,0xd1,0x0a,0x65,0x05,0x5c,0x3d};

    memset(in, 0, sizeof(in));
    memset(out, 0, sizeof(out));
    memset(back, 0, sizeof(back));
    
    memcpy(key,origin_key,8);
    DES_set_key((DES_cblock *)key, &keysched);
    DES_set_key_checked((DES_cblock *)key, &keysched);
 
    /* 8 bytes of plaintext */
    strcpy(in, clear);
 
    printf("Plaintext: [%s]\n", in);
    unsigned char* base64_in;
    base64_in = (unsigned char*)malloc(sizeof(char)*BUFSIZE);
    memset(base64_in,0,BUFSIZE);

    base64_in = base64(in,clear_len);
    e= base64_in;
    printf("Plaintext with base64: [%s]\n", base64_in);

    int i=0;
    int iter=0;
    if(clear_len/16 * 16 != clear_len){
        iter = (clear_len/16+1)* 16;
    }else iter = (clear_len/16) *16;
    printf("iter : %d\n",iter);
    if(clear_len<iter) {
        //padding
        memset(in+clear_len,0x0,iter-clear_len);
    }
    for(i=0; i<iter; i+=8){
        DES_ecb_encrypt((DES_cblock *)(in+i),(DES_cblock *)(out+i), &keysched, DES_ENCRYPT);
    }

    base64_in = base64(out,iter);
    printf("des with base64:\n");
    printf("%s\n",base64_in);
    //print_out_hexa(base64_in);

    AES_KEY enc_key_128;


    unsigned char aes_128_key[] = {0x00,0x01,0x24,0x53,
            0x50,0xb5,0xeb,0x0a,0x15,0x48,0xfc,0x6d,0x27,0xd3,0xb4,0xd1};

	unsigned char iv[AES_BLOCK_SIZE];
	memset(iv, 0x00, AES_BLOCK_SIZE);
// clear to be out
    unsigned char cipher[BUFSIZE];
    memset(cipher, 0, sizeof(cipher));
    memset(back, 0, sizeof(back));

    AES_set_encrypt_key(aes_128_key, sizeof(aes_128_key)*8, &enc_key_128);
    AES_cbc_encrypt(out,cipher, sizeof(out), &enc_key_128, iv, AES_ENCRYPT);
    unsigned char* base64_cipher = base64(cipher,iter);
    print_out_hexa(cipher);
    printf("Encrypted Text with DES,AES base64: [%s]\n", base64_cipher); 

    free(base64_in);
    return 0;
}
void print_out_hexa(unsigned char* e){
    int i=0;
    while (*e) {
        i++;
        printf("%02x ", *e++);
        //printf("%c",*e++);
    }    
    printf("\ni : %d\n",i);
}
void print_out_char(unsigned char* e){
    int i=0;
    while (*e) {
        i++;
        printf("%c",*e++);
    }    
    printf("\ni : %d\n",i);
}
char *base64(const unsigned char *input, int length)
{
  BIO *bmem, *b64;
  BUF_MEM *bptr;

  b64 = BIO_new(BIO_f_base64());
  bmem = BIO_new(BIO_s_mem());
  b64 = BIO_push(b64, bmem);
  BIO_write(b64, input, length);
  BIO_flush(b64);
  BIO_get_mem_ptr(b64, &bptr);

  char *buff = (char *)malloc(bptr->length);
  memcpy(buff, bptr->data, bptr->length-1);
  buff[bptr->length-1] = 0;

  BIO_free_all(b64);

  return buff;
}

