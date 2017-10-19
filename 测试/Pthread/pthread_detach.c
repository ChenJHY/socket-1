/*************************************************************************
	> File Name: pthread_detach.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月19日 星期二 15时27分48秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void * th_fn(void *arg)
{
    int i;
    int sum = 0;
    for(i = 1; i <= 100; i++){
        sum += 1;
    }
    return (void*)sum;
}

void out_state(pthread_attr_t *attr)
{
    int state;
    if(pthread_attr_getdetachstate(attr, &state) != 0){
        perror("getdetachstate error");
    }
    else{
        if(state == PTHREAD_CREATE_JOINABLE){
            printf("joinable state\n");
        }
        else if(state == PTHREAD_CREATE_DETACHED){
            printf("detached state\n");
        }
        else{
            printf("error state\n");
        }
    }
}

int main(void)
{
    int err;
    pthread_t default_th, detach_th;
    //定义线程的属性
    pthread_attr_t attr;
    //对线程属性初始化
    pthread_attr_init(&attr);
    //输出分离属性
    out_state(&attr);

    if((err = pthread_create(&default_th, &attr, th_fn, (void*)0)) != 0){
        perror("pthread create error");
    }

    int res;
    if((err = pthread_join(default_th, (void*)&res)) != 0){
        perror("pthread join error");
    }
    else{
        printf("default return is %d\n", (int)res);
    }

    printf("----------------------------------------\n");

    //设置分离属性为分离状态
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    out_state(&attr);
    //分离状态启动子线程

    if((err = pthread_create(&detach_th, &attr, th_fn, (void*)0)) != 0){
        perror("pthread create error");
    }
    if((err = pthread_join(detach_th, (void*)&res)) != 0){
        //perror("pthread join error");
        fprintf(stderr, "%s\n", strerror(err));
    }
    else{
        printf("default return is %d\n", (int)res);
    }

    pthread_attr_destroy(&attr);
    printf("0x%lx finished\n", pthread_self());
    sleep(1);

    return 0;
}
