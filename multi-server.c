#include "addr.h"


int main(int argc, char **argv)
{
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
        while(1){
                int pid;
                connfd = accept(sockfd, (struct sockaddr*)&client, &clen);
                pid = fork();
                if(pid < 0) exit(2);
                if(pid == 0){   // child
                        close(sockfd);
                        // printf("[child] close sockfd\n");
                        char buffer[1024];
                        memset(buffer,0,sizeof(buffer));
                        while(recv(connfd,buffer,1024-1,0)>0);
                        printf("recv: %s\n",buffer);
                        close(connfd);
                        printf("[child] child exit\n");
                        exit(0);
                } else { // parent
                        // printf("[parent] parent close connfd\n");
                        close(connfd);
                }
        }// end while 1
        return 0;
}

