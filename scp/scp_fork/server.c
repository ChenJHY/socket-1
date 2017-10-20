/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2017年10月11日 星期三 17时20分31秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <wait.h>
#include <errno.h>

#define PORT 6000
#define LISTENQ 20
#define BUFFSIZE 4096
#define FILE_NAME_MAX_SIZE 512

int main(int argc, char *argv[])
{
    char fileName[FILE_NAME_MAX_SIZE];
    bzero(fileName, FILE_NAME_MAX_SIZE);
    printf("Please input the file name you want to send: ");
    scanf("%s", &fileName);
    getchar();

    int sockfd, connfd;
    struct sockaddr_in serverAddr, clientAddr;
    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket error");
        exit(1);
    }

    if(bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        perror("bind error");
        exit(1);
    }

    if(listen(sockfd, LISTENQ) < 0){
        perror("listen error");
        exit(1);
    }

    pid_t pid;

    while(1){
        socklen_t length = sizeof(clientAddr);

        if((connfd = accept(sockfd, (struct sockaddr*)&clientAddr, &length)) < 0){
            perror("accept error");
            exit(1);
        }
        if((pid = fork()) == 0){
            char buff[BUFFSIZE];
            int count;
            bzero(buff, BUFFSIZE);

            strncpy(buff, fileName, strlen(fileName) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(fileName));

            if((count = send(connfd, buff, BUFFSIZE, 0)) < 0){
                perror("send file information error");
                exit(1);
            }

            FILE *fd = fopen(fileName, "rb");
            if(NULL == fd){
                printf("File: %s not found!\n", fileName);
                int judge = 1;
                if(send(connfd, &judge, sizeof(judge), 0) < 0){
                    perror("send judge error");
                    exit(1);
                }
                close(connfd);
                exit(1);
            }else{
                int judge = 0;
                if(send(connfd, &judge, sizeof(judge), 0) < 0){
                    perror("send judge error");
                    exit(1);
                }

                printf("\nstart transfering md5!\n");
                char cmd[BUFFSIZE] = "md5sum ";
                strcat(cmd, fileName);
        
                bzero(buff, BUFFSIZE);
                FILE *stream = popen(cmd, "r");
                int tmp = 0; 
                if((tmp = fread(buff, sizeof(char), BUFFSIZE, stream)) <= 0){
                    perror("fread md5 error");
                    exit(1);
                }

                if(send(connfd, buff, tmp, 0) < 0){
                    perror("send md5 error");
                    exit(1);
                }

                pclose(stream);
                printf("Transfer md5 finished!\n");
    

                bzero(buff, BUFFSIZE);
                int file_block_length = 0;

                while((file_block_length = fread(buff, sizeof(char), BUFFSIZE, fd)) > 0){
                    printf("file_block_length: %d\n", file_block_length);

                    if(send(connfd, buff, file_block_length, 0) < 0){
                        perror("send error");
                        exit(1);
                    }
                    if(errno == EPIPE){
                        break;
                    }

                    bzero(buff, BUFFSIZE);
                }

                fclose(fd);
                printf("Transfer file finished!\n");
            }
            close(connfd);
        }
        else if(pid > 0){
            close(connfd);
            wait(NULL);
        }
        else{
            perror("fork error");
            exit(1);
        }
    }

    return 0;
}
