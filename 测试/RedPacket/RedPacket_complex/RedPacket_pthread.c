/*************************************************************************
	> File Name: RedPacket_pthread.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月21日 星期四 22时45分34秒
 ************************************************************************/

#include<stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "RedPacket.h"

typedef struct
{
    char name[20];
    RedPacket *redpacket;
}Account;

void get_rand_str(char s[], int num)
{
    char *str = "0123456789abcdefghigklmnopqrstuvwxyz";
    int i, lstr;
    char ss[2] = {0};

    lstr = strlen(str);
    srand((unsigned int)time((time_t *)(NULL)));
    for(i = 1; i <= num; i++)
    {
        sprintf(ss, "%c", str[(rand()%lstr)]);
        strcat(s, ss);
    }
}

void *withdraw(void *arg)
{
    Account *a = (Account*)arg;
    get_rand_str(a->name, 10);
    
    double amt = red_read(a->redpacket);
    printf("the amt is %f", amt);
    
    return (void*)0;
}

void *deposit(void *arg)
{
    Account *a = (Account*)arg;
    int n = a->redpacket->count;
    double m = a->redpacket->money;
    printf("please input count and money!\n");
    scanf("%d, %f", &n, &m);
    red_write(n, m);
    return (void*)0;
}

int main(void)
{
    int err;
    pthread_t write, read[5];
    RedPacket *red = (RedPacket*)malloc(sizeof(RedPacket)); 

    Account cus;
    cus.redpacket = red;
    seminit(red);
    
    if((err = pthread_create(&write, NULL, deposit, (void*)&cus)) != 0){
        perror("pthread create error");
    }

    for(int i = 1; i <= 5; i++){
        if((err = pthread_create(&read[i], NULL, withdraw, (void*)&cus)) != 0){
            perror("pthread create error");
        }
    }

    for(int j = 1; j <= 5; j++){
        pthread_join(read[j], NULL);
    }
    pthread_join(write, NULL);
    red_destroy(red);

    return 0;
}
