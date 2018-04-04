#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>

#define BUFSIZE 1024

char *path = "/run/mysock.sock";

int main(int argc, char **argv)
{
        struct sockaddr_un address;
        int unix_sockfd;
        int ret;
        memset(&address, 0 , sizeof(struct sockaddr_un));
        address.sun_family = AF_UNIX;
        // address.sun_path = path;
        strncpy(address.sun_path, path, 16);
        unlink(path);
        unix_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
        ret = bind(unix_sockfd,(struct sockaddr*)&address, sizeof(address));
        if (ret == -1){
            perror("bind error\n");
            exit(1);
        }
        ret = listen(unix_sockfd, 5);
        if (ret == -1){
            perror("listen error\n");
            exit(3);
        }

        int client_sockfd;
        struct sockaddr_un client;
        int clientlen = sizeof(client);
        while(1){
                client_sockfd = accept(unix_sockfd,(struct sockaddr*)&client, &clientlen);
                if (client_sockfd == -1){
                        perror("accept error\n");
                        exit(2);
                }
                char buffer[BUFSIZE];
                memset(buffer,0,BUFSIZE);
                while(recv(client_sockfd,buffer,BUFSIZE-1,0)>0);
                printf("recv: %s\n",buffer);
                close(client_sockfd);
        }
        close(unix_sockfd);
        return 0;
}
