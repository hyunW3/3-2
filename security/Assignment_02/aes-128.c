#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>



#define BUFSIZE 64

// AES-128 cbc encrypt
//https://wikidocs.net/78832
// 
char *base64(const unsigned char *input, int length);

//0001245350b5eb0a1548fc6d27d3b4d1 piewtf
int main(){
    AES_KEY enc_key_128;
    unsigned char aes_128_key[] = {0x00,0x00,0x00,0x1,0x2,0x4,0x5,
                0x3,0x5,0x0,0xb,0x5,0xe,0xb,0x0,
                0xa,0x1,0x5,0x4,0x8,0xf,0xc,0x6,
                0xd,0x2,0x7,0xd,0x3,0xb,0x4,0xd,0x1};
	unsigned char iv[AES_BLOCK_SIZE];
	memset(iv, 0x00, AES_BLOCK_SIZE);
    int i = 0;
/*
    unsigned char clear[] = {'69','39','27','13 ','a3','1c','c5',
                '49','7c','2f','8c','b4','23',
                    '5d','07','84','b2','53','bf','b6','86','f9','3d', 
                    'cd','0e','7a','81','94','bc','67','b5','ea','ba', 
                    'aa','eb','b3','79','5f','c1','e5','77','b3','8c', 
                    '6f','aa','c0','27','c7'};
*/
    unsigned char clear[] = {0x69,0x39,0x27,0x13,0xa3,0x1c,0xc5,
    0x49,0x7c,0x2f,0x8c,0xb4,0x23,
    0x5d,0x07,0x84,0xb2,0x53,0xbf,
    0xb6,0x86,0xf9,0x3d,0xcd,0x0e,
    0x7a,0x81,0x94,0xbc,0x67,0xb5,
    0xea,0xba,0xaa,0xeb,0xb3,0x79,
    0x5f,0xc1,0xe5,0x77,0xb3,0x8c,
    0x6f,0xaa,0xc0,0x27,0xc7};
    unsigned char cipher[BUFSIZE];
    unsigned char back[BUFSIZE];
    //memset(clear, 0, sizeof(clear));
    memset(cipher, 0, sizeof(cipher));
    memset(back, 0, sizeof(back));
    unsigned char* e = clear;
    while(*e) printf("%02x ",*e++);
    printf("\n");
    AES_set_encrypt_key(aes_128_key, sizeof(aes_128_key)*8, &enc_key_128);
    AES_cbc_encrypt(clear,cipher, sizeof(clear), &enc_key_128, iv, AES_ENCRYPT);
    
    printf("ciper:\n");
    e = cipher;
    while(*e) printf("%02x ",*e++);  //printf("%c ",*e++);
    printf("\n");
	memset(iv, 0x00, AES_BLOCK_SIZE);

    unsigned char* base64_cipher;
    base64_cipher = (unsigned char*)malloc(sizeof(char)*BUFSIZE);
    memset(base64_cipher,0,BUFSIZE);
    base64_cipher = base64(cipher,sizeof(clear));
    e= base64_cipher;
    printf("base64 encoding:\n");
   
    while(*e) {
        i++;
        printf("%c",*e++);
    }
    printf("\ni : %d\n",i);
    AES_KEY dec_key_128;
    AES_set_decrypt_key(aes_128_key, sizeof(aes_128_key)*8, &dec_key_128);
    AES_cbc_encrypt(cipher,back, sizeof(clear), &dec_key_128, iv, AES_DECRYPT);
    printf("backward:\n");
    e = back;
    //while(*e) printf("%02x ",*e++);  
    //printf("\n");
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
