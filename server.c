#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

int main(int argc, char **argv)
{
    char *ip = "127.0.0.1";
    int port = 18080;
    int ret;
    int sockfd;
    struct sockaddr_in address;
    // memset(address,0,sizeof(struct sockaddr_in));
    bzero(&address,sizeof(struct sockaddr));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    assert(sockfd >= 0);

    int reuse = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    ret = bind(sockfd,(struct sockaddr*)&address,sizeof(struct sockaddr));
    if (ret == -1){
        printf("bind error: %d\n",errno);
    }
    ret = listen(sockfd, 5);
    if (ret == -1){
        printf("listen error: %d\n",errno);
    }
    // recv from client;
    struct sockaddr_in client;
    int clen = sizeof(client);
    int connfd;
    bzero(&client,sizeof(struct sockaddr));
    connfd = accept(sockfd,(struct sockaddr*)&client,&clen);
    if (connfd == -1){
        printf("accept error: %d\n",errno);
    } else {
        char buffer[1024];
        memset(&buffer,0,sizeof(buffer));
        while(recv(connfd,buffer,1024-1,0)>0);
        printf("recv: %s\n",buffer);
        close(connfd);
    }
    close(sockfd);
    return 0;
}

