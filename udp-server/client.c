
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
    address.sin_addr.s_addr = inet_addr(ip);
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    char *buffer = "Hello server\n";
    char *abc;
    printf("buffer size: %ld\n",sizeof(buffer));
    int addrlen = sizeof(address);
    sendto(sockfd, buffer, 13, 0, (struct sockaddr*)&address, addrlen);
    printf("send buffer: %s\n", buffer);
    close(sockfd);
    return 0;
}
