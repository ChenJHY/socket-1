/*************************************************************************
	> File Name: process_wait.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月12日 星期二 18时40分59秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void out_status(int status){
    if(WIFEXITED(status)){
        printf("normal exit: %d\n", WEXITSTATUS(status));
    }
    else if(WIFSIGNALED(status)){
        printf("abnormal term: %d\n", WTERMSIG(status));
    }
    else if(WIFSTOPPED(status)){
        printf("stopped sig: %d\n", WSTOPSIG(status));
        //kill -19 测试 
    }
    else{
        printf("unknow sig\n");
    }
}

int main(void){

    int status;
    pid_t pid;

    if((pid = fork()) < 0){
        perror("fork error");
        exit(1);
    }
    else if(pid == 0){
        printf("pid: %d, ppid: %d\n", getpid(), getppid());
        exit(3); //子进程终止运行
    }

    //父进程阻塞，等待子进程结束并回收
    wait(&status);
    out_status(status);

    printf("--------------------------\n");

    if((pid = fork()) < 0){
        perror("fork error");
        exit(1);
    }
    else if(pid == 0){
        printf("pid: %d, ppid: %d\n", getpid(), getppid());

        int i = 3, j = 0;
        int k = i / j;
        printf("k: %d\n", k);
    }

    wait(&status);
    out_status(status);
    printf("--------------------------\n");


    if((pid = fork()) < 0){
        perror("fork error");
        exit(1);
    }
    else if(pid == 0){
        printf("pid: %d, ppid: %d\n", getpid(), getppid());
        pause();
    }

    do{
        pid = waitpid(pid, &status, WNOHANG | WUNTRACED);

        if(pid == 0){
            sleep(1);
        }
    }while(pid == 0);
    out_status(status);

    return 0;
}
