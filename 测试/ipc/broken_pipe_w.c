/*************************************************************************
	> File Name: broken_pipe_w.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月27日 星期三 19时46分23秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

/*
 *不完整管道：写一个读端已经被关闭的管道
 */

void sig_handler(int signo)
{
    if(signo == SIGPIPE){
        printf("SIGPIPE occured\n");
    }
}

int main(void)
{
    int fd[2];
    if(pipe(fd) < 0){
        perror("pipe error");
        exit(1);
    }

    pid_t pid;
    if((pid = fork()) < 0){
        perror("fork error");
        exit(1);
    }else if(pid > 0){
        //父进程写入数据到不完整的管道
        sleep(3);
        close(fd[0]);
        if(signal(SIGPIPE, sig_handler) == SIG_ERR){
            perror("signal pipe error");
            exit(1);
        }

        char *s= "1234";
        if(write(fd[1], s, sizeof(s)) != sizeof(s)){
            fprintf(stderr, "%s, %s\n", strerror(errno), (errno == EPIPE) ? "EPIPE" : ",unknow");
        }

        close(fd[1]);
        wait(NULL);
    }else{
        //子进程关闭管道的读端
        close(fd[0]);
        close(fd[1]);
    }
}
