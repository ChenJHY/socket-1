/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2017年10月07日 星期六 15时31分19秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX 1024
#define LISTEN 5
#define FILE_NAME 20

int main(int argc, char *argv[])
{
    char *nameBuf = (char*)malloc(sizeof(char) * FILE_NAME);
    bzero(nameBuf, FILE_NAME);

    if(argc < 2){
        perror("param error");
        exit(1);
    }
    nameBuf = argv[1];

    int fd;
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket error");
        exit(1);
    }

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8090);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("bind error");
        exit(1);
    }
    if(listen(fd, LISTEN) < 0){
        perror("listen error");
        exit(1);
    }

    struct sockaddr_in cliaddr;
    int connfd;

    char buf[MAX];
    int count;
    
    while(1){
        socklen_t cli_len = sizeof(cliaddr);
        if((connfd = accept(fd, (struct sockaddr*)&cliaddr, &cli_len)) < 0){
            perror("accept");
            exit(1);
        }

        bzero(buf, MAX);
        strncpy(buf, nameBuf, strlen(nameBuf) > FILE_NAME ? FILE_NAME : strlen(nameBuf));

        if((count = send(connfd, buf, MAX, 0)) < 0){
            perror("send");
            exit(1);
        }

        FILE *fp = fopen(nameBuf, "rb");
        if(NULL == fp){
            perror("fopen");
            exit(1);
        }else{
            bzero(buf, MAX);
            int fileLen = 0;

            while((fileLen = fread(buf, sizeof(char), MAX, fp)) > 0){
                printf("finishen!\n");

                if(send(connfd, buf, fileLen, 0) < 0){
                    perror("send");
                    exit(1);
                }
                bzero(buf, MAX);
            }
            fclose(fp);
        }
        close(connfd);
    }
    close(fd);

    return 0;
}
