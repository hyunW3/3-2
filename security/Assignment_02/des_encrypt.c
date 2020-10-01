#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/des.h>
#include <openssl/aes.h>
 
#define BUFSIZE 64
 
int main(void)
{
    // in -> out -> back(restore)
    unsigned char in[BUFSIZE], out[BUFSIZE], back[BUFSIZE];
    unsigned char *e = out;
 
    DES_cblock key;
    //DES_cblock seed = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    DES_key_schedule keysched;
    char clear[] = "SKKU is the top university in the world";
    // 000099656c16f8e8 4f50d10a65055c3 d
    //unsigned char origin_key[] = {'0','0','0','0','9','9','6',
    //                     '5','6','c','1','6','f','8','e','8'};
    unsigned char origin_key[] = {'0','0','0','0','9','9','6',
                         '5','6','c','1','6','f','8','e','8',
                         '4','f','5','0','d','1','0','a','6',
                         '5','0','5','5','c','3'};

    memset(in, 0, sizeof(in));
    memset(out, 0, sizeof(out));
    memset(back, 0, sizeof(back));
    //RAND_seed(seed, sizeof(DES_cblock));
 
    //DES_random_key(&key);
    memcpy(key,origin_key,sizeof(origin_key));
    DES_set_key((DES_cblock *)key, &keysched);
 
    /* 8 bytes of plaintext */
    strcpy(in, clear);
    //memset(in+40,0,8);
 
    printf("Plaintext: [%s]\n", in);


    int i=0;
    int iter=0;
    if(sizeof(clear)/16 * 16 != sizeof(clear)){
        iter = (sizeof(clear)/16+1)* 16;
    }else iter = (sizeof(clear)/16) *16;
    printf("iter : %d\n",iter);
    if(sizeof(clear)<iter) {
        //padding
        memset(in+sizeof(clear),0x0,iter-sizeof(clear));
    }
    for(i=0; i<iter; i+=8){
        DES_ecb_encrypt((DES_cblock *)(in+i),(DES_cblock *)(out+i), &keysched, DES_ENCRYPT);
    }
 
    printf("i:%d\nCiphertext:",i);
    i=0;
    while (*e) {
        i++;
        //printf("%02x ", *e++);
        printf("%c",*e++);
    }
    printf("\ni : %d\n",i);
 
    for(int i=0; i<=sizeof(in); i+=8){
        DES_ecb_encrypt((DES_cblock *)(out+i),(DES_cblock *)(back+i), &keysched, DES_DECRYPT);
    }
    
    printf("Decrypted Text: [%s]\n", back);

 // DES end

    //out -> out2 -> back(restore)
    unsigned char out2[BUFSIZE];
    memset(back, 0, sizeof(back));
    memset(out2, 0, sizeof(out2));

    AES_KEY enc_key_128;
    unsigned char aes_128_key[] = {0x00,0x00,0x00,0x1,0x2,0x4,0x5,
                0x3,0x5,0x0,0xb,0x5,0xe,0xb,0x0,
                0xa,0x1,0x5,0x4,0x8,0xf,0xc,0x6,
                0xd,0x2,0x7,0xd,0x3,0xb,0x4,0xd,0x1};
	unsigned char iv[AES_BLOCK_SIZE];
	memset(iv, 0x00, AES_BLOCK_SIZE);
    unsigned char cipher[16];

    return(0);
}

// http://blog.naver.com/PostView.nhn?blogId=sysganda&logNo=30126780360&categoryNo=30&parentCategoryNo=0&viewDate=&currentPage=1&postListTopCurrentPage=1&from=postView