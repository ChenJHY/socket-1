/*************************************************************************
	> File Name: fifo_write.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月27日 星期三 21时31分52秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("usage: %s fifo \n", argv[0]);
        exit(1);
    }
    printf("open fifo write...\n");

    int fd = open(argv[1], O_WRONLY);
    if(fd < 0){
        perror("open error");
        exit(1);
    }else{
        printf("open fifo success: %d\n", fd);
    }

    char *s = "123456789";
    size_t size = strlen(s);
    if(write(fd, s, size) != size){
        perror("write error");
    }
    close(fd);

    exit(0);
}
