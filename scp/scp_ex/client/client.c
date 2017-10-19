/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2017年10月07日 星期六 15时31分11秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX 1024
#define FILE_NAME 20

int main(void)
{
    int fd;
    char buf[MAX];
    struct sockaddr_in servaddr;

    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket error");
        exit(1);
    }
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8090);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(connect(fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("connect error");
        exit(1);
    }    

    
    char nameBuf[MAX];
    int count;
    if((count = recv(fd, buf, sizeof(nameBuf), 0)) < 0){
        perror("recv");
        exit(1);
    }

    strncpy(nameBuf, buf, strlen(buf) > FILE_NAME ? FILE_NAME : strlen(buf));

    FILE *fp = fopen(nameBuf, "wb+");
    if(NULL == fp){
        perror("fopen");
        exit(1);
    }
    bzero(buf, MAX);

    int len;
    while((len = recv(fd, buf, sizeof(buf), 0)) > 0){
        if(len < 0){
            perror("recv");
            exit(1);
        }

        int wlen = 0;
        if((wlen = fwrite(buf, sizeof(char), len, fp)) < len){
            perror("fwrite");
            exit(1);
        }
        bzero(buf, MAX);
    }

    fclose(fp);
    close(fd);

    return 0;
}
