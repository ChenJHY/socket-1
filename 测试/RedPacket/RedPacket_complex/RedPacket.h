/*************************************************************************
	> File Name: RedPacket.h
	> Author: 
	> Mail: 
	> Created Time: 2017年09月21日 星期四 20时33分05秒
 ************************************************************************/

#ifndef _REDPACKET_H
#define _REDPACKET_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include <time.h>
#include <semaphore.h>

typedef struct
{
    int count; //红包的个数
    double money; //红包的金额
    double amt; //红包余额

    sem_t sem; //定义条件变量
}RedPacket;

double Random(double m, double n);

void red_write(int count, double money);

double red_read(RedPacket *arg);

void red_destroy(RedPacket *arg);

void seminit(RedPacket *arg);

#endif
