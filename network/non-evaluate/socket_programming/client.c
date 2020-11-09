#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char* argv[]){
    int client_sock;

    struct sockaddr_in serv_addr;
    
    char message[100];
    int strlen;
    
    if(argc != 3){
        printf("Usage : %s <IP> <port>\n",argv[0]);
        return -1;
    }
    client_sock = socket(PF_INET,SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    puts("start to connect"); 
    if(connect(client_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <0) {
        puts("connect error");
        return -1;
    }
    puts("wait to read"); 
    strlen = read(client_sock, message, sizeof(message)-1);

    if(strlen == -1) return -1;

    printf("Message from server : %s\n",message);
    close(client_sock);

    return 0;


}