/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月31日 星期四 19时22分54秒
 ************************************************************************/

#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <math.h>

#define PORT 60000
#define MAXLINE 1024
#define max(a, b) (a)>(b)?(a):(b)

void str_cli(FILE *fp, int sockfd);

int main(int argc, char **argv){
    int sockfd;
    struct sockaddr_in servaddr;

    if(argc != 2){
        perror("IpAddress!\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    str_cli(stdin, sockfd);

    exit(0);
}


void str_cli(FILE *fp, int sockfd){
    int maxfdp, stdineof;
    fd_set rset;
    char buf[MAXLINE];
    int n;
    stdineof = 0;
    
    FD_ZERO(&rset);
    while(1){
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp = max(fileno(fp), sockfd) + 1;

        select(maxfdp, &rset, NULL, NULL, NULL);

        if(FD_ISSET(sockfd, &rset)){
            
            if(read(sockfd, buf, MAXLINE) == 0){
                if(stdineof == 1){
                    return;
                }
                else{
                    perror("recv error!\n");
                    exit(1);
                }
            }
           write(fileno(stdout), buf, n);
        }

        if(FD_ISSET(fileno(fp), &rset)){
            
            if((n = read(fileno(fp), buf, MAXLINE)) == 0){
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            write(sockfd, buf, n);
        }
    }
}
