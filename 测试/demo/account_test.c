/*************************************************************************
	> File Name: account_test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月19日 星期二 17时08分21秒
 ************************************************************************/

#include<stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "account.h"

typedef struct
{
    char name[20];
    Account *account;
    double amt;
}OperArg;


//定义取款操作的线程运行函数
void *withdraw_fn(void *arg)
{
    OperArg *oa = (OperArg*)arg;
    double amt = withdraw(oa->account, oa->amt);
    printf("%s(0x%lx) withdraw %f from account %d\n",
           oa->name, pthread_self(), amt, oa->account->code);

    return (void*)0;
}

//定义一个存款操作的线程运行函数
void *deposit_fn(void *arg)
{
    OperArg *oa = (OperArg*)arg;
    double amt = deposit(oa->account, oa->amt);
    printf("%s(0x%lx) deposit %f from account %d\n",
           oa->name, pthread_self(), amt, oa->account->code);

    return (void*)0;
}

//定义一个检查银行账户的线程运行函数
void *check_fn(void *arg)
{
    OperArg *oa = (OperArg*)arg;
    double balance = get_balance(oa->account);
    printf("%s(0x%lx) balance is %f\n",oa->name, balance);
    return (void*)0;
}

int main(void)
{
    int err;
    pthread_t boy, girl;
    Account *a = create_account(100001, 10000);
    
    OperArg o1, o2;
    strcpy(o1.name, "boy");
    o1.account = a;
    o1.amt = 10000;

    strcpy(o2.name, "girl");
    o2.account = a;
    o2.amt = 10000;

    //启动两个线程同时操作银行账户

    if((err = pthread_create(&boy, NULL, withdraw_fn, (void*)&o1)) != 0){
        perror("pthread create error");
    }
    
    if((err = pthread_create(&girl, NULL, withdraw_fn, (void*)&o2)) != 0){
        perror("pthread create error");
    }
   
    /*
    printf("--------------------------------\n");

    if((err = pthread_create(&boy, NULL, check_fn, (void*)&o1)) != 0){
        perror("pthread create error");
    }
    
    if((err = pthread_create(&girl, NULL, check_fn, (void*)&o2)) != 0){
        perror("pthread create error");
    }
    */

    pthread_join(boy, NULL);
    pthread_join(girl, NULL);

    printf("account balance: %f\n", get_balance(a));
    destroy_account(a);

    return 0;
}
