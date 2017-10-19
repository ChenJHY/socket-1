/*************************************************************************
	> File Name: sem_test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月20日 星期三 22时39分36秒
 ************************************************************************/

#include<stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>

sem_t sem1;
sem_t sem2;

void* a_fn(void *arg)
{
    sem_wait(&sem1);
    printf("thread a running\n");   
 
    return (void*)0;
}

void* b_fn(void *arg)
{
    sem_wait(&sem2);
    printf("thread b running\n");   
    //释放线程信号变量sem1
    sem_post(&sem1);

    return (void*)0;
}

void* c_fn(void *arg)
{
    printf("thread c running\n");   
    //释放线程b(对信号量sem2做加一操作）
    sem_post(&sem2);

    return (void*)0;
}

int main(void)
{
    pthread_t a, b, c;
    //线程信号量初始化
    sem_init(&sem1, 0, 0);  //第二个参数是是否在进程间共享的标志，0为不共享，1为共享
    sem_init(&sem2, 0, 0);

    pthread_create(&a, NULL, a_fn, (void*)0);
    pthread_create(&b, NULL, b_fn, (void*)0);
    pthread_create(&c, NULL, c_fn, (void*)0);

    pthread_join(a, NULL);
    pthread_join(b, NULL);
    pthread_join(c, NULL);

    //销毁线程信号量
    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return 0;
}
