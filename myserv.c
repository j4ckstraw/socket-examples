#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <string.h>

#define BUFSIZE 1024

char *ip = "127.0.0.1";
int port = 18080;

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in address;
    
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY; 
    // address.sin_addr.s_addr = inet_addr(ip); 
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sockfd, (struct sockaddr*)&address, sizeof(address));
    listen(sockfd,5);
    
    unsigned char buf[sizeof(struct in_addr)];
    printf("listen on: %s:%d\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port));
    while(1){
        int connfd;
        int pid;
        struct sockaddr_in client;
        int clientlen = sizeof(client);
        memset(&client,'\0',sizeof(client));
        connfd = accept(sockfd, (struct sockaddr*)&client, &clientlen);
        if(connfd == -1) {
            perror("accept error\n");
            exit(2);
        }
        pid = fork();
        if(pid < 0) {
            perror("fork error\n");
            exit(2);
        }
        if(pid == 0) { // child
            char buffer[BUFSIZE];
            memset(buffer,'\0',sizeof(buffer));
            close(sockfd);
            printf("[child] close sockfd\n");
            while(recv(connfd,buffer,BUFSIZE-1,0)>0);
            printf("%s:%d connected\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
            printf("data: %s\n",buffer);
            exit(0);
        } else { //parent
            int status;
            int ret;
            close(connfd); 
            ret = wait(&status);  // you need wait for child to exit.
            printf("child %d exit status: %d\n",ret,status);
        }
    }//end while 1
    return 0;
}
