/*************************************************************************
	> File Name: RedPacket.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月21日 星期四 21时04分56秒
 ************************************************************************/

#include<stdio.h>
#include "RedPacket.h"

double Random(double m, double n){
    int pos, dis;
    double temp;
    if(m == n){
        return m;
    }
    else if(m > n){
        do{
            pos = (int)n;
            dis = (int)(m - n);
            double tem = drand48();
            temp = rand()%dis + pos + tem;
        }while(temp <= m);
        return temp;
    }
    else{
        do{
            pos = (int)m;
            dis = (int)(n - m);
            double tem = drand48();
            temp = rand()%dis + pos + tem;
        }while(temp <= n);
        return temp;
    }
}

void seminit(RedPacket *arg)
{
    sem_init(&arg->sem, 0, 0);
}

void red_write(int count, double money)
{
    RedPacket *tem = (RedPacket*)malloc(sizeof(RedPacket));
    assert(tem != NULL);
    tem->count = count;
    tem->money = money;
    tem->amt = money;
    sem_post(&tem->sem);
}

double red_read(RedPacket *arg)
{
    assert(arg != NULL);
    sem_wait(&arg->sem);

    if(0 == (arg->count--)){
        printf("(%lx) get %lf\n", pthread_self(), arg->amt);
        arg->amt = 0.0;
        sem_post(&arg->sem);
    }
    else{
        srand((double)time(NULL));
        double m = arg->amt;
        double n = 0.0;
        double x = Random(m, n);

        printf("(%lx) get %lf\n", pthread_self(), x);
        arg->amt -= x;
        
        sem_post(&arg->sem);
    }

    return arg->amt;
}

void red_destroy(RedPacket *arg)
{
    assert(arg != NULL);
    sem_destroy(&arg->sem);
    free(arg);
}
