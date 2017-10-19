/*************************************************************************
	> File Name: signal_alarm.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月14日 星期四 20时23分54秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>

void sig_handler(int signo)
{
    if(signo == SIGALRM){
        printf("clock time out\n");
        alarm(5);
    }
}

void out_data(void)
{
    int i = 1;
    while(i <= 20){
        double d = drand48();
        printf("%-10d:%lf\n", i++, d);
        if(i == 16)
            alarm(0);  //取消定时器
        sleep(1);
    }
}

int main(void)
{
    if(signal(SIGALRM, sig_handler) == SIG_ERR){
        perror("signal sihalrm error");
    }

    alarm(5);

    printf("begin running main\n");

    out_data();

    printf("end running main\n");

    return 0;
}
