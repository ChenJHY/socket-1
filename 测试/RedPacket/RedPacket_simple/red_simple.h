/*************************************************************************
	> File Name: red_simple.h
	> Author: 
	> Mail: 
	> Created Time: 2017年09月23日 星期六 21时21分06秒
 ************************************************************************/

#ifndef _RED_SIMPLE_H
#define _RED_SIMPLE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <malloc.h>

typedef double ElemenType;
#define MIN 0
    
pthread_mutex_t mutex; //定义互斥锁

int number;

typedef struct{
    int count; //红包个数
    ElemenType money; //红包金额
    ElemenType amt; //红包余额
}red;

typedef struct{
    char name[5];
    int have;
    red *r_cus;
}cus;

typedef struct{
    char name[5];
    red *r_pro;
}pro;

cus *r_c;

ElemenType Random(ElemenType money, int count)
{
    /*随机金额函数*/
    srand((unsigned)time(NULL));
    ElemenType agv = money / count; //获取平均值
    ElemenType tmp = agv * 2;
    //初始化随机数
    ElemenType res = (rand() % (int)(res * 100) + (int)(MIN * 100)) / 100.0 + MIN;
    
    return res;
}

void* red_get(void *arg)
{
    //cus *c = (cus*)arg;
    int c = *(int*)arg;
    pthread_mutex_lock(&mutex);
    printf("the %d:", c);

    if(r_c[c].r_cus->money > 0.0 && r_c[c].have == 0){
        if(r_c[c].r_cus->count > 1){
            ElemenType randmoney = Random(r_c[c].r_cus->money, r_c[c].r_cus->count);  
            
            printf("%s 取走 %f 元\n", r_c[c].name, randmoney);
            r_c[c].r_cus->money -= randmoney;
            r_c[c].r_cus->amt = r_c[c].r_cus->money;

            printf("剩余 %f \n", r_c[c].r_cus->amt);

            r_c[c].have = 1;
            r_c[c].r_cus->count--;
        }else{
            printf("%s 取走 %f 元\n", r_c[c].name, r_c[c].r_cus->money);
            printf("剩余 0 元\n");
            r_c[c].have = 1;
            r_c[c].r_cus->count--;
        }
    }

    pthread_mutex_unlock(&mutex);
    pthread_cancel(pthread_self());
}

void* red_give(void *arg)
{
    pro *p = (pro*)arg;
    pthread_mutex_lock(&mutex);

    while(1){
        printf("please input money: ");
        scanf("%lf", &p->r_pro->money);
        printf("please input the number: ");
        scanf("%d", &p->r_pro->count);
        p->r_pro->amt = p->r_pro->money;

        number = p->r_pro->count;

        if(p->r_pro->money > MIN && p->r_pro->count >= 1){
            printf("-------------------------------\n");
            break;
        }else{
            printf("error: please input again\n");
            printf("-------------------------------\n");
        }
    }


    pthread_mutex_unlock(&mutex);

    return (void*)0;
}

#endif
