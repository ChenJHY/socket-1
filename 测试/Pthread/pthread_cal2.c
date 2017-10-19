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

/*
 *一个线程负责计算结果，一个获取结果
 *获取结果必须等待计算完成，然后通知获取结果的线程获取结果
 */

typedef struct
{
    int res;  //存储计算的结果
    int count;  //用来统计获取结果的线程的数量
    pthread_cond_t cond;  //定义条件变量
    pthread_mutex_t mutex;  //定义互斥锁
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
 
    pthread_mutex_lock(&r->mutex);

    //判断获取结果的线程是达到指定的数量
    while(r->count < 2){
        pthread_mutex_unlock(&r->mutex);
        printf("usleep!\n");
        usleep(100);
        pthread_mutex_lock(&r->mutex);
    }
    pthread_mutex_unlock(&r->mutex);

    //唤醒等待的获取结果的线程
    pthread_cond_broadcast(&r->cond);

    return (void*)0;
}

//获取结果的线程函数
void *get_fn(void *arg)
{
    Result *r = (Result*)arg;
 
    //对两个线程共享的判断条件进行保护
    pthread_mutex_lock(&r->mutex);

    //代表获取的线程准备好了
    r->count++;

    //获取结果的线程进行等待(阻塞)
    pthread_cond_wait(&r->cond, &r->mutex);
    //函数内部流程:第一步先unlock(&mutex)
    //然后lock(&mutex)(防止其他线程的操作)
    //然后将线程自己插入到条件变量的等待队列中
    //然后unlock(&mutex)
    //然后阻塞等待别的线程来唤醒
    //唤醒后，调用lock(&mutex)
    //从等待队列中删除自己


    //被唤醒后解锁
    pthread_mutex_unlock(&r->mutex);

    //获取计算的结果
    int res = r->res;
    printf("0x%lx get sum is: %d\n", pthread_self(), res);

    return (void*)0;
}

int main(void)
{
    int err;
    pthread_t cal, get, get1, get2;
    Result r;

    r.count = 0;
    pthread_cond_init(&r.cond, NULL);
    pthread_mutex_init(&r.mutex, NULL);

    if((err = pthread_create(&get, NULL, get_fn, (void*)&r) != 0)){
        perror("pthread create error");
    }
    
    if((err = pthread_create(&get1, NULL, get_fn, (void*)&r) != 0)){
        perror("pthread create error");
    }
    
    if((err = pthread_create(&get2, NULL, get_fn, (void*)&r) != 0)){
        perror("pthread create error");
    }
    
    if((err = pthread_create(&cal, NULL, set_fn, (void*)&r) != 0)){
        perror("pthread create error");
    }

    pthread_join(cal, NULL);
    pthread_join(get, NULL);
    pthread_join(get1, NULL);
    pthread_join(get2, NULL);

    pthread_cond_destroy(&r.cond);
    pthread_mutex_destroy(&r.mutex);

    return 0;
}
