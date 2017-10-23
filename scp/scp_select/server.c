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

    int reuse = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0){
        perror("setsockopt error");
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

    int client[FD_SETSIZE];
    memset(client, -1, FD_SETSIZE);
    fd_set sock_set;
    fd_set write_set;
    int i = 0;
    int maxfd = sockfd;
    int maxi = -1;
    int nready = 0;

    while(1){
        FD_ZERO(&sock_set);
        FD_ZERO(&write_set);
        FD_SET(sockfd, &sock_set);
        FD_SET(sockfd, &write_set);

        if((nready = select(maxfd+1, &sock_set, &write_set, NULL, NULL)) < 0){
            perror("select error");
            exit(1);
        }

        if(FD_ISSET(sockfd, &sock_set)){
            socklen_t length = sizeof(clientAddr);

            if((connfd = accept(sockfd, (struct sockaddr*)&clientAddr, &length)) < 0){
                perror("accept error");
                exit(1);
            }

            for(i = 0; i < FD_SETSIZE; i++){
                if(client[i] < 0){
                    client[i] = connfd;
                    i++;  //可以注释，则113行改为注释的代码
                    break;
                }
            }
            if(i == FD_SETSIZE){
                printf("too many clients!\n");
                exit(1);
            }

            FD_SET(connfd, &write_set);
            if(connfd > maxfd){
                maxfd = connfd;
            }
            if(i > maxi){
                maxi = i;
            }
            if(--nready < 0){
                continue;
            }
        }
        for(i = 0; i < maxi; i++){  //for(i = 0; i <= maxi; i++){
            if(client[i] < 0){
                continue;
            }

            if(FD_ISSET(client[i], &write_set)){
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
                close(client[i]);
                FD_CLR(client[i], &write_set);
                client[i] = -1;

                if(--nready <= 0){
                    break;
                }
            }
        }
    }

    return 0;
}
