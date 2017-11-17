/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月13日 星期一 20时06分55秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#define MAXSIZE 1024
#define PORT 55555

int main()
{
    int sockfd;
    struct sockaddr_in clientAddr;

    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(PORT);
    clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(1);
    }

    if(connect(sockfd, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0){
        perror("connect");
        exit(1);
    }

    char buf[MAXSIZE];
    bzero(buf, MAXSIZE);

    struct pollfd server[2];

    server[0].fd = sockfd;
    server[0].events = POLLIN;
    server[1].fd = fileno(stdin);
    server[1].events = POLLIN;

    int nready;
    int maxfd = sockfd > fileno(stdin)?sockfd:fileno(stdin);

    while(1){
        if((nready = poll(server, maxfd+1, -1)) < 0 ){
            perror("poll");
            exit(1);
        }
        if(nready == 0)
            continue;

        if(server[1].revents & POLLIN){
            if(fgets(buf, MAXSIZE, stdin) != NULL){
                if(send(sockfd, buf, strlen(buf), 0) < 0){
                    perror("send");
                    exit(1);
                }
                bzero(buf, MAXSIZE);
            }
        }

        if(server[0].revents & POLLIN){
            int n = 0;
            if((n = recv(sockfd, buf, MAXSIZE, 0)) < 0){
                perror("recv");
                exit(1);
            }else if(n == 0){
                close(sockfd);
            }else{
                printf("\nserver : %s", buf);
                putchar(10);
            }
        }
    }
    close(sockfd);
    
    return 0;
}
