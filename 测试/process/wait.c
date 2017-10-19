/*************************************************************************
	> File Name: wait.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月30日 星期三 20时30分37秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
    pid_t pid;
    int status, exit_status;

    if((pid = fork()) < 0){
        perror("fork error!\n");
        exit(1);
    }

    if(!pid){
        sleep(4);
        exit(5);  //using value is not 0   fang bian guan cha
    }

    if(wait(&status) < 0){
        perror("wait failed!\n");
        exit(1);
    }

    if(status & 0xFF){
        printf("Somne low-roderbits not zero\n");
    }
    else{
        exit_status = status >> 8;
        exit_status &= 0xFF;
        printf("Exit status from %d was %d\n", pid, exit_status);
    }

    exit(0);
}
