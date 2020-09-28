#include <stdio.h>

#define KEYLENGTH 8


int main(){
    unsigned char ch;
    FILE *fpIn, *fpOut;
    //unsigned char key[KEYLENGTH] = {0x11, 0x22};
    //unsigned char key[KEYLENGTH] = { 0x01, 0x02, 0x03};
    //unsigned char key[KEYLENGTH] = { 0x01, 0x02, 0x01}; //3
    //unsigned char key[KEYLENGTH] = { 0x01, 0x02, 0x03, 0x04}; 
    //unsigned char key[KEYLENGTH] = { 0x01, 0x02, 0x03, 0x04,0x05}; //5
    //unsigned char key[KEYLENGTH] = { 0x01, 0x02, 0x03, 0x04,0x05,0x06}; //6
    //unsigned char key[KEYLENGTH] = { 0x11, 0x22, 0x01, 0x43,0x05,0x02, 0x03}; //7
    unsigned char key[KEYLENGTH] = { 0x11, 0x22, 0x01, 0x03,0x05,0x02, 0x03,0x05}; //8
    //unsigned char key[KEYLENGTH] = { 0x01, 0x03, 0x01, 0x03, 0x01,0x03, 0x03,0x05}; //8 error 
    //unsigned char key[KEYLENGTH] = { 0x11, 0x22, 0x01, 0x43,0x05,0x02, 0x03,0x04,0x05}; //9
    //unsigned char key[KEYLENGTH] = { 0x11, 0x22, 0x01, 0x43,0x05,0x02, 0x03,0x04,0x05,0x01}; //10
    /* key values can be changed */

    fpIn = fopen("plain_text.txt","r");
    fpOut = fopen("hw1_input.txt", "wb");
    for(int i=0; fscanf(fpIn, "%c", &ch) != EOF; ++i){
        ch ^= key[i%KEYLENGTH];
        //printf("%c",ch);
        fwrite(&ch, sizeof(ch), 1, fpOut);
    }
    fclose(fpIn);
    fclose(fpOut);
    return 0;
}