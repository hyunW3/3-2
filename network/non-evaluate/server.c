#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char* argv[]){
    int serv_sock;
    int client_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;
    int client_addr_size;

    char message[] = "hello world";

    if(argc != 2){
        printf("Usage is wrong : %s <port>\n",argv[0]);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        puts("bind error\n");
        return -1;
    }

    listen(serv_sock,5);
    puts("Waits to accept");
    client_addr_size = sizeof(client_addr);
    client_sock = accept(serv_sock, (struct sockaddr *)&client_addr, &client_addr_size);

    write(client_sock, message, sizeof(message));
    close(client_sock);
    close(serv_sock);

    return 0;

}