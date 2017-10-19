/*************************************************************************
	> File Name: pthread_term.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月18日 星期一 20时37分01秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct{
    int d1;
    int d2;
}Arg;

void *th_fn(void *arg)
{
    Arg *r = (Arg*)arg;

    //return (void*)(r->d1 + r->d2);
    return (void*)r;
}

int main()
{
    int err;
    pthread_t th;
    Arg r = {20, 50};

    if((err = pthread_create(&th, NULL, th_fn, (void*)&r)) != 0){
        perror("pthread_create error");
    }

    /*
    int *result;
    pthread_join(th, (void**)&result);
    printf("result is %d\n", (int)result);
    */

    /*
    int result;
    pthread_join(th, (void*)&result);
    printf("result is %d\n", (int)result);
    */

    int *result;
    pthread_join(th, (void**)&result);
    printf("result is %d\n", ((Arg*)result)->d1 + ((Arg*)result)->d2);

    return 0;
}
