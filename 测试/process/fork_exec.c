/*************************************************************************
	> File Name: fork_exec.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月30日 星期三 19时19分32秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main(){
    pid_t pid;
    pid = fork();
    switch(pid){
        case -1:
            perror("fork error!\n");
            exit(1);
        case 0:
            execl("/bin/ls", "ls", "-l", "--color", NULL);
            perror("execl error!\n");
            exit(1);
        default:
            wait(NULL);
            printf("ls finished!\n");
            exit(0);
    }
    exit(1);
}
