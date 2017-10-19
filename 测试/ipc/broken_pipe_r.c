/*************************************************************************
	> File Name: broken_pipe_r.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月27日 星期三 19时31分43秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/*
 * 非完整管道：
 *读取一个写端已经关闭的管道
 */

int main(void)
{
    int fd[2];
    if(pipe(fd) < 0){
        perror("pipepe error");
        exit(1);
    }

    pid_t pid;
    if((pid = fork()) < 0){
        perror("fork error");
        exit(1);
    }else if(pid > 0){
        //父进程从不完整管道中读取数据
        sleep(3); //等待子进程将管道的写端关闭
        close(fd[1]);

        while(1){
            char c;
            if(read(fd[0], &c, 1) == 0){
                printf("\nwrite-end of pipe closed\n");
                break;
            }else{
                printf("%c", c);
            }
        }

        close(fd[0]);
        wait(NULL);
    }else{
        //子进程负责将数据写入到管道
        close(fd[0]);
        char *s = "1234";
        write(fd[1], s, sizeof(s));
        //写入后关闭管道的写端
        close(fd[1]);
    }

    exit(0);
}
