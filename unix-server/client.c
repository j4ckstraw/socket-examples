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
        int addr_len = sizeof(address);
        int unix_sockfd;
        int ret;
        memset(&address, 0 , sizeof(struct sockaddr_un));
        address.sun_family = AF_UNIX;
        // address.sun_path = path;
        strncpy(address.sun_path, path, 16);

        unix_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
        ret = connect(unix_sockfd,(struct sockaddr*)&address,addr_len);
        if (ret == -1 ){
            perror("connect error\n");
            exit(2);
        }
        char *buffer = "hello server\n";
        int buf_len = 13;
        ret = send(unix_sockfd,buffer,buf_len,0);
        if (ret < 0 ){
            perror("send error\n");
            exit(3);
        }
        close(unix_sockfd);
        return 0;
}
