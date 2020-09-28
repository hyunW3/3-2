#include <stdio.h>
#include <stdlib.h>

#define KEYLENGTH 8

//EOF 0x1A

int main(){
    unsigned char ch;
    FILE *fpIn, *fpOut;
    char* buf;
    //unsigned char key[KEYLENGTH] = {0x01, 0x02};
    //unsigned char key[KEYLENGTH] = { 0x01, 0x02, 0x03};
    //unsigned char key[KEYLENGTH] = { 0x01, 0x02, 0x03, 0x04};
    //unsigned char key[KEYLENGTH] = { 0x11, 0x22, 0x01, 0x43,0x05,0x02, 0x03}; //7
    unsigned char key[KEYLENGTH] = { 0x11, 0x22, 0x01, 0x03,0x05,0x02, 0x03,0x05}; //8
    //unsigned char key[KEYLENGTH] = { 0x01, 0x03, 0x01, 0x03, 0x01,0x03, 0x02,0x05}; //8 error
    //unsigned char key[KEYLENGTH] = { 0x11, 0x22, 0x01, 0x43,0x05,0x02, 0x03,0x04,0x05}; //9
    //unsigned char key[KEYLENGTH] = { 0x11, 0x22, 0x01, 0x43,0x05,0x02, 0x03,0x04,0x05,0x01}; //10
    /* key values can be changed */

    fpIn = fopen("hw1_input.txt","r");
    fpOut = fopen("decryption_text.txt", "wb");
    fseek(fpIn,0,SEEK_END);
    int file_length = ftell(fpIn);
    fseek(fpIn,0,SEEK_SET);
    buf = (char*)malloc(sizeof(char)*(file_length+1));
    fread(buf,file_length, sizeof(char), fpIn);
    //for(int i=0; fscanf(fpIn, "%c", &ch) != EOF; ++i){
    for(int i=0; i<file_length; ++i){
        ch = buf[i];
       // printf("%c",ch);
        
        ch ^= key[i%KEYLENGTH];
        fwrite(&ch, sizeof(ch), 1, fpOut);
    }
    fclose(fpIn);
    fclose(fpOut);
    return 0;
}