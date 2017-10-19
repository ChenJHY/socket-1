/*************************************************************************
	> File Name: account.h
	> Author: 
	> Mail: 
	> Created Time: 2017年09月19日 星期二 16时51分48秒
 ************************************************************************/

#ifndef _ACCOUNT_H
#define _ACCOUNT_H
#include <pthread.h>
#include <semaphore.h>

typedef struct{
    int code;  //号码
    double balance;  //余额
    //建议互斥锁和账户绑定在一起，否则出现一把锁锁多个账户，导致并发性降低
    //pthread_mutex_t mutex;  //定义一把互斥锁
    //pthread_rwlock_t rwlock;
    //定义信号量
    sem_t sem;
}Account;

//创建账户
extern Account* create_account(int code, double balance);
//销毁账户
extern void destroy_account(Account *a);
//取款
extern double withdraw(Account *a, double amt);
//存款
extern double deposit(Account *a, double amt);
//查看余额
extern double get_balance(Account *a);

#endif
