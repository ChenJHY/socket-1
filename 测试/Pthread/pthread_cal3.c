/*************************************************************************
	> File Name: pthread_cal.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月20日 星期三 17时03分08秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

/*
 *一个线程负责计算结果，一个获取结果
 *获取结果必须等待计算完成，然后通知获取结果的线程获取结果
 */

typedef struct
{
    int res;  //存储计算的结果
    sem_t sem;
}Result;

//计算并将结果放到Result中的线程函数
void *set_fn(void *arg)
{
    Result *r = (Result*)arg;
    int i = 1;
    int sum = 0;
    
    for(; i <= 100; i++){
        sum += i;
    }
    r->res = sum;
    
    //V(1)操作
    sem_post(&r->sem);

    return (void*)0;
}

//获取结果的线程函数
void *get_fn(void *arg)
{
    Result *r = (Result*)arg;
 
    //P(1)操作
    sem_wait(&r->sem);

    //获取计算的结果
    int res = r->res;
    printf("0x%lx get sum is: %d\n", pthread_self(), res);

    return (void*)0;
}

int main(void)
{
    int err;
    pthread_t cal, get;
    Result r;
    sem_init(&r.sem, 0, 0);

    if((err = pthread_create(&get, NULL, get_fn, (void*)&r) != 0)){
        perror("pthread create error");
    }
    
    if((err = pthread_create(&cal, NULL, set_fn, (void*)&r) != 0)){
        perror("pthread create error");
    }

    pthread_join(cal, NULL);
    pthread_join(get, NULL);

    sem_destroy(&r.sem);

    return 0;
}
