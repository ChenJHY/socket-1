/*************************************************************************
	> File Name: cal_pipe.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月27日 星期三 15时55分13秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//父进程通过管道传输两个数据给子进程
//子进程读取打印


int main(void)
{
    int fd[2];

    //创建管道
    if(pipe(fd) < 0){
        perror("pipe error");
        exit(1);
    }

    pid_t pid;

    if((pid = fork()) < 0){
        perror("fork error");
        exit(1);
    }else if(pid > 0){
        //父进程写入数据
        close(fd[0]);
        int start = 1, end = 2;

        if(write(fd[1], &start, sizeof(int)) != sizeof(int)){
            perror("write error");
            exit(1);
        }
        if(write(fd[1], &end, sizeof(int)) != sizeof(int)){
            perror("write error");
            exit(1);
        }

        close(fd[1]);

        wait(0);
    }else{
        //子进程读取数据
        close(fd[1]);
        int start, end;
        if(read(fd[0], &start, sizeof(int)) < 0){
            perror("read error");
            exit(1);
        }
        if(read(fd[0], &end, sizeof(int)) < 0){
            perror("read error");
            exit(1);
        }

        close(fd[0]);
        printf("child process read start: %d, end: %d\n", start, end);
    }

    return 0;
}
