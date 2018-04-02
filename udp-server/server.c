#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFSIZE 1024

char *ip = "127.0.0.1";
int port = 18080;

int main(int argc, char **argv)
{
    struct sockaddr_in address;
    int sockfd;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (struct sockaddr*)&address, sizeof(address));
    char buffer[BUFSIZE];
    struct sockaddr_in client;
    int clientlen = sizeof(client);
    memset(&buffer,'\0',sizeof(buffer));
    while(recvfrom(sockfd, buffer, BUFSIZE-1, 0, (struct sockaddr*)&client, &clientlen)>0)
        printf("receive from %s:%d\nbuffer: %s\n",inet_ntoa(client.sin_addr), htons(client.sin_port), buffer);
    close(sockfd);
    return 0;
}
