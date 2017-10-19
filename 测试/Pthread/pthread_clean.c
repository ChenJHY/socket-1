/*************************************************************************
	> File Name: pthread_clean.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月18日 星期一 21时26分00秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void clean_fun(void *arg)
{
    char *s = (char*)arg;
    printf("clean_fun: %s\n", s);
}

void *th_fun(void *arg)
{
    int execute = (int)arg;
    pthread_cleanup_push(clean_fun, "first clean");
    pthread_cleanup_push(clean_fun, "second clean");
    
    printf("thread running %lx\n", pthread_self());

    pthread_cleanup_pop(execute);
    pthread_cleanup_pop(execute);

    return (void*)0;
}

int main(void)
{
    int err;
    pthread_t th1, th2;

    if((err = pthread_create(&th1, NULL, th_fun, (void*)1)) != 0){
        perror("pthread_create error");
    }

    pthread_join(th1, NULL);
    printf("th1(%lx) finished\n", th1);
    
    if((err = pthread_create(&th2, NULL, th_fun, (void*)1)) != 0){
        perror("pthread_create error");
    }

    pthread_join(th2, NULL);
    printf("th2(%lx) finished\n", th2);
}
