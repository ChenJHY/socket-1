/*************************************************************************
	> File Name: signal_catch.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月13日 星期三 22时04分11秒
 ************************************************************************/

#include<stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void sig_handler(int signo){
    printf("%d, %d occrued\n", getpid(), signo);
}

int main(void){
    if(signal(SIGTSTP, sig_handler) == SIG_ERR){
        perror("signal sigtstp error");
    }
    if(signal(SIGINT, sig_handler) == SIG_ERR){
        perror("signal sigint error");
    }

    int i = 0;
    while(i < 30){
        printf("%d out %d\n", getpid(), i++);
        sleep(1);
    }

    return 0;
}
