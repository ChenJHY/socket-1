/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2017年10月11日 星期三 17时20分20秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 6000
#define LISTENQ 20
#define BUFFSIZE 4096
#define FILE_NAME_MAX_SIZE 512

int main(int argc, char *argv[])
{
    int clientfd;

    if(argc != 2){
        fprintf(stderr, "usage: ./fileclient <IP_Address>\n");
        exit(1);
    }

    struct sockaddr_in clientAddr;
    bzero(&clientAddr, sizeof(clientAddr));

    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddr.sin_port = htons(PORT);
    
    if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket error");
        exit(1);
    }


    if(connect(clientfd, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0){
        perror("connect error");
        exit(1);
    }

    char buff[BUFFSIZE];
    char fileName[FILE_NAME_MAX_SIZE];
    bzero(fileName, FILE_NAME_MAX_SIZE);

    int count;
    bzero(buff, BUFFSIZE);

    if((count = recv(clientfd, buff, BUFFSIZE, 0)) < 0){
        perror("recv error");
        exit(1);
    }
    
    strncpy(fileName, buff, strlen(buff) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buff));

    printf("Preparing receive file: %s --from-- %s\n", fileName, argv[1]);

    FILE *fd = fopen(fileName, "wb+");
    if(NULL == fd){
        perror("open error");
        exit(1);
    }
    bzero(buff, BUFFSIZE);

    int length = 0;
    while(length = recv(clientfd, buff, BUFFSIZE, 0)){
        if(length < 0){
            perror("recv");
            exit(1);
        }

        int writeLen = fwrite(buff, sizeof(char), length, fd);
        if(writeLen < length){
            perror("write error");
            exit(1);
        }
        bzero(buff, BUFFSIZE);
    }

    printf("Received file: %s --from-- %s!\n", fileName, argv[1]);

    fclose(fd);
    close(clientfd);

    return 0; 
}
