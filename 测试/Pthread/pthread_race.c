/*************************************************************************
	> File Name: pthread_race.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月14日 星期四 21时43分37秒
 ************************************************************************/

#include<stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

void *th_fn(void *arg)
{
    int distance = (int)arg;
    int i;
    for(i = 1; i <= distance; i++){
        printf("%lx run %d\n", pthread_self(), i);
        int time = (int)(drand48() * 100000);
        usleep(time);
    }

    return (void*)0;
}


int main(void)
{
    int err;
    pthread_t rabbit, turtle;  //定义线程标识符

    if((err = pthread_create(&rabbit, NULL, th_fn, (void*)50)) != 0){
        perror("pthread_create error");
    }
    
    if((err = pthread_create(&turtle, NULL, th_fn, (void*)50)) != 0){
        perror("pthread_create error");
    }

    //主控线程调用pthread_join，自己会阻塞，直到所等待的线程结束方可运行
    pthread_join(rabbit, NULL);
    pthread_join(turtle, NULL);
    //sleep(3);
    printf("control thread id: %lx\n", pthread_self());
    printf("finished!\n");

    return 0;
}
