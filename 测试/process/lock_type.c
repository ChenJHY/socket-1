/*************************************************************************
	> File Name: lock_type.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月19日 星期二 20时15分06秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    pthread_mutex_t mutex;

    if(argc < 2){
        printf("usage: %s [error|normal|recursive]\n", argv[0]);
        exit(1);
    }

    //定义互斥锁属性
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init(&mutexattr);

    if(!strcmp(argv[1], "error")){
        pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK);  
    }
    else if(!strcmp(argv[1], "normal")){
        pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_NORMAL);
    }
    else if(!strcmp(argv[1], "recursive")){
        pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
    }

    pthread_mutex_init(&mutex, &mutexattr);

    if(pthread_mutex_lock(&mutex) != 0){
        printf("lock failure\n");
    }
    else{
        printf("lock success\n");
    }
    
    if(pthread_mutex_lock(&mutex) != 0){
        printf("lock failure\n");
    }
    else{
        printf("lock success\n");
    }

    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex);

    pthread_mutexattr_destroy(&mutexattr);
    pthread_mutex_destroy(&mutex);

    return 0;
}
