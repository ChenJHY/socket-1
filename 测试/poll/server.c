/*************************************************************************
	> File Name: sever.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月13日 星期一 20时06分49秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define MAXSIZE 1024  //缓冲区最大值
#define PORT 55555  //端口号
#define LISTENQ 20  //监听的最大值
#define INFTIM -1  //timeout时间

int main(int argc, char *argv[])
{
    int listenfd;
    struct sockaddr_in serverAddr, clientAddr;
    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(1);
    }

    //地址复用
    int reuse = 1;
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0){
        perror("setsockopt");
        exit(1);
    }

    if(bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        perror("bind");
        exit(1);
    }

    if(listen(listenfd, LISTENQ) < 0){
        perror("listen");
        exit(1);
    }

    //获取每个进程最大打开的文件数
    long OPEN_MAX = sysconf(_SC_OPEN_MAX);

    struct pollfd client[OPEN_MAX];
    
    //注册监听事件
    client[0].fd = listenfd;
    client[0].events = POLLIN;
    int i = 1;
    for(; i < OPEN_MAX; i++){
        client[i].fd = -1;
    }

    int sockfd, connfd;
    int nready;
    int maxi = 0;
    char buf[MAXSIZE];
    bzero(buf, MAXSIZE);
    socklen_t clilen;

    while(1){
        if((nready = poll(client, maxi+1, INFTIM)) < 0){
            perror("poll");
            exit(1);
        }

        if(client[0].revents & POLLIN){
            clilen = sizeof(clientAddr);
            if((connfd = accept(listenfd, (struct sockaddr*)&clientAddr, &clilen)) < 0){
                perror("accept");
                exit(1);
            }

            for(i = 1; i < OPEN_MAX; i++){
                if(client[i].fd < 0){
                    client[i].fd = connfd;
                    client[i].events = POLLIN;
                    break;
                }
            }

            if(i == OPEN_MAX){
                perror("too many clients");
                exit(1);
            }

            if(i > maxi){
                maxi = i;
            }

            if(--nready <= 0){
                continue;
            }
        }

        for(i = 1; i <= maxi; i++){
            if((sockfd = client[i].fd) < 0){
                continue;
            }

            bzero(buf, MAXSIZE);
            if(client[i].revents & (POLLIN | POLLERR)){
                int n = 0;
                if((n = recv(sockfd, buf, MAXSIZE, 0)) < 0){
                    if(errno == ECONNRESET){
                        close(sockfd);
                        client[i].fd = -1;
                    }else{
                        perror("recv");
                        exit(1);
                    }
                }else if(n == 0){
                    close(sockfd);
                    client[i].fd = -1;
                }else if(n > 0){
                    printf("client : %s", buf);
                    if(send(sockfd, buf, strlen(buf), 0) < 0){
                        perror("send");
                        exit(1);
                    }
                    printf("\nserver : %s", buf);
                    putchar(10);
                }

                if(--nready <= 0){
                    break;
                }
            }
        }
    }
    
    return 0;
}
