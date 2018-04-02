//服务端
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
        if (argc <= 3) {
                printf("Usage: %s ip port revc_size\n", basename(argv[0]));
                return 1;
        }

        const char *ip = argv[1];
        int port = atoi(argv[2]);

        struct sockaddr_in address;
        bzero(&address, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        inet_pton(AF_INET, ip, &address.sin_addr);

        int sock = socket(PF_INET, SOCK_STREAM, 0);
        assert(sock >= 0);

        //设置地址可重用
        int reuse = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

        int recvbuf = atoi(argv[3]);
        int len = sizeof(recvbuf);

        //设置接受缓冲区大小
        setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, sizeof(recvbuf));
        //获取系统修改后的大小
        getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, (socklen_t*)&len);
        printf("the receive buffer size after setting is %d\n", recvbuf);

        int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
        assert(ret != -1);

        ret = listen(sock, 5);
        assert(ret != -1);

        struct sockaddr_in client;
        socklen_t client_addrlength = sizeof(client);

        int connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);
        if (connfd < 0) {
                printf("errno is: %d\n", errno);
        }
        else {
                char buffer[BUFFER_SIZE];
                memset(buffer, '\0', BUFFER_SIZE);

                while (recv(connfd, buffer, BUFFER_SIZE-1, 0) > 0);

                printf("recv: %s\n", buffer);
                close(connfd);
        }

        close(sock);

        return 0;
}
