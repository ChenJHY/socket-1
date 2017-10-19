/*************************************************************************
	> File Name: account.h
	> Author: 
	> Mail: 
	> Created Time: 2017年09月19日 星期二 16时51分48秒
 ************************************************************************/
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "account.h"

//创建账户
Account* create_account(int code, double balance)
{
    Account *a = (Account*)malloc(sizeof(Account));
    assert(a != NULL);
    a->code = code;
    a->balance = balance;

    //pthread_mutex_init(&a->mutex, NULL);

    //pthread_rwlock_init(&a->rwlock, NULL);
    
    sem_init(&a->sem, 0, 1);
    return a;
}

//销毁账户
void destroy_account(Account *a)
{
    assert(a != NULL);
    //pthread_mutex_destroy(&a->mutex);

    //pthread_rwlock_destroy(&a->rwlock);
 
    sem_destroy(&a->sem);
    free(a);
}

//取款
double withdraw(Account *a, double amt)
{
    assert(a != NULL);

    //pthread_mutex_lock(&a->mutex);
    
    //P（1）操作
    sem_wait(&a->sem);

    if(amt < 0 || amt > a->balance){
        //pthread_mutex_unlock(&a->mutex);
        
        //V(1)操作
        sem_post(&a->sem);
        return 0.0;
    }

    double balance = a->balance;
    sleep(1);

    balance -= amt;
    a->balance = balance;
    
    //pthread_mutex_unlock(&a->mutex);
    sem_post(&a->sem);
    
    return amt;
}

//存款
double deposit(Account *a, double amt)
{
    assert(a != NULL);

    //pthread_mutex_lock(&a->mutex);
    
    sem_wait(&a->sem);

    if(amt < 0){
        //pthread_mutex_unlock(&a->mutex);
        
        sem_post(&a->sem);
        return 0.0;
    }

    double balance = a->balance;
    sleep(1);

    balance += amt;
    a->balance = balance;
    
    //pthread_mutex_unlock(&a->mutex);
    sem_post(&a->sem);
    
    return balance;
}

//查看余额
double get_balance(Account *a)
{
    assert(a != NULL);
    //pthread_rwlock_rdlock(&a->rwlock);
 
    sem_wait(&a->sem);

    double balance = a->balance;
    
    //pthread_rwlock_unlock(&a->rwlock);
    
    sem_post(&a->sem);

    return balance;
}
