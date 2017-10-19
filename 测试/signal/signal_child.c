/*************************************************************************
	> File Name: signal_child.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月14日 星期四 19时43分12秒
 ************************************************************************/

#include<stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void sig_handler(int signo){
    printf("child process deaded, signo %d\n", signo);
    //当父进程捕获到SIGCHLD信号后，要调用wait函数，回收子进程，避免僵尸进程
    wait(0);
}

void out(int n){
    int i;
    for(i = 0; i < n; i++){
        printf("%d out %d\n", getpid(), i);
        sleep(1);
    }
}

int main(void){
    if(signal(SIGCHLD, sig_handler) == SIG_ERR){
        perror("signal sigchld error");
    }

    pid_t pid = fork();

    if(pid < 0){
        perror("fork error");
        exit(1);
    }
    else if(pid > 0){
        out(100);
    }
    else{
        out(10);
    }
}
