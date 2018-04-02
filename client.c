#include "addr.h"

int main(int argc, char **argv)
{
    struct sockaddr_in address;
    int sockfd;
    int ret;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ret = connect(sockfd,(struct sockaddr*)&address,sizeof(address));
    if (ret == -1){
        printf("connect error: %d\n",errno);
    } else {
        char buffer[20] = "hello server\n";
        // memset(buffer,'\0',sizeof(buffer));
        printf("buffer: %s\nsize:%d\n",buffer,sizeof(buffer));
        send(sockfd,buffer,sizeof(buffer),0);
    }
    close(sockfd);
    return 0;
}
