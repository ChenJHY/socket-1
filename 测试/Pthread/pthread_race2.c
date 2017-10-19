/*************************************************************************
	> File Name: pthread_race2.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月18日 星期一 19时37分41秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
    char name[20];
    int time;
    int start;
    int end;
}RaceArg;

void *th_fn(void *arg)
{
    RaceArg *r = (RaceArg*)arg;
    int i = r->start;

    for(; i < r->end; i++){
        printf("%s(%lx) running %d\n", r->name, pthread_self(), i);
        usleep(r->time);
    }

    return (void*)0;  //pthread_exit((void*)0);
}


int main(void)
{
    int err;
    pthread_t rabbit, turtle;

    RaceArg r_a = 
        {"rabbit", (int)(drand48()*100000000), 20, 50};
    RaceArg t_a = 
        {"turtle", (int)(drand48()*100000000), 10, 60};

    if((err = pthread_create(&rabbit, NULL, th_fn, (void*)&r_a) != 0)){
        perror("pthread_creat error");
    }
    if((err = pthread_create(&turtle, NULL, th_fn, (void*)&t_a) != 0)){
        perror("pthread_creat error");
    }

    pthread_join(rabbit, NULL);
    pthread_join(turtle, NULL);

    printf("control thread id: %lx\n", pthread_self());
    printf("finished\n");

    return 0;
}
