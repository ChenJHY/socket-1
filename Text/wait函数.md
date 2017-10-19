---
title: wait函数和waitpid函数
date: 2017/10/11
categories: 
	- 网络编程
tags: 
	- 网络编程
	- wait函数
---

### wait函数和waitpid函数

#### 1. 僵尸进程

1. 说明

	1. 子进程结束但是没有完全释放内存(在内核中的task_struct没有释放)，该进程就会成为僵尸进程
	2. 当僵尸进程的父进程结束后就会被init进程(1号进程)接管，最终被回收

2. 僵尸进程的危害

	1. 如果你不处理僵尸进程的话，那么保留的那段信息就不会释放，其进程号就会一定被占用，但是系统所能使用的进程号是有限的，如果大量的产生僵尸进程，将因为没有可用的进程号而导致系统不能产生新的进程

3. 避免僵尸进程

	1. 让僵尸进程的父进程来回收，父进程每隔一段时间来查询子进程是否结束并被回收，调用wait或者waitpid函数，通知内核释放僵尸进程
	2. 采用信号SIGCHLD通知处理，并在信号处理程序中调用wait函数
	3. 让僵尸进程成为孤儿进程，并有init进程回收


#### 2.避免僵尸进程方法一说明

- 头文件

```c
	#include <sys/types.h>
	#include <sys/wait.h>
```

- 区别

	1. 在一个进程终止前，wait 使其调用者阻塞
	2. waitpid 函数有一个选择项，可以使调用者不阻塞
	3. waitpid 等待一个指定的子进程，wait 等待所有的子进程，返回任一子进程的终止状态

- 参数

	1. **status参数**
		1. 为空时，代表任意状态结束的子进程，若不为空，则代表指定状态结束的子进程

	2. **options参数**
		1. **WNOHANG：**若由pid指定的子进程没有退出则立即返回，则waitpid不阻塞，此时返回值为0
		2. **WUNTRACED：**若某实现支持作业控制，则由pid指定的任一子进程状态已暂停，且其状态自暂停以来还未报告过，则返回其状态

	3. 检查 wait 和 waitpid 函数返回终止状态的宏(前面判断，后面获得状态码)
		1. **WIFEXITED/WEXITSTATUS(status)**：若为正常终止子进程的返回的状态，则为真
		2. **WIFSIGNALED/WTERMSIG(status)**：若为异常终止子进程的返回的状态，则为真(接到一个不能捕捉的信号)
		3. **WIFSTOPED/WSTOPSIG(status)**：若为当前暂停子进程的返回的状态，则为真(如果当前进程在终止前暂停过，则获得暂停的状态码)

#### 2. wait函数

1. 原型： `pid_t wait(int *status)`
2. 返回：成功返回子进程ID，出错返回-1
3. 作用：等待子进程退出并回收，防止僵尸进程产生


#### 3. waitpid函数

1. 原型：`pid_t waitpid(pid_t pid, int *status, int options)`
2. 返回：成功返回子进程ID，出错返回-1
3. 功能：wait函数的非阻塞版本
4. pid参数：
	1. pid == -1：等待任一子进程，与功能 wait 相等
	2. pid > 0：等待其进程ID与 pid 相等的子进程
	3. pid == 0：等待其组ID等于调用进程的组ID的任一子进程
	4. pid < -1：等待其组ID等于 pid 的绝对值的任一子进程

#### 4. 示例

```c
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
	#include <sys/wait.h>
	#include <sys/types.h>

	void out_status(int status){
	    if(WIFEXITED(status)){
	    	//正常终止
	        printf("normal exit: %d\n", WEXITSTATUS(status));
	    }
	    else if(WIFSIGNALED(status)){
	    	//异常终止
	        printf("abnormal term: %d\n", WTERMSIG(status));
	    }
	    else if(WIFSTOPPED(status)){
	    	//终止前暂停或者等待过
	        printf("stopped sig: %d\n", WSTOPSIG(status));
	        //kill -19 测试结果
	    }
	    else{
	        printf("unknow sig\n");
	    }
	}

	int main(void){

	    int status;
	    pid_t pid;

	    if((pid = fork()) < 0){
	        perror("fork error");
	        exit(1);
	    }
	    else if(pid == 0){
	        printf("pid: %d, ppid: %d\n", getpid(), getppid());
	        exit(3); //子进程终止运行
	    }

	    //父进程阻塞，等待子进程结束并回收
	    wait(&status);
	    out_status(status);

	    printf("--------------------------\n");

	    if((pid = fork()) < 0){
	        perror("fork error");
	        exit(1);
	    }
	    else if(pid == 0){
	        printf("pid: %d, ppid: %d\n", getpid(), getppid());

	        int i = 3, j = 0;
	        int k = i / j;  //异常测试
	        printf("k: %d\n", k);
	    }

	    wait(&status);
	    out_status(status);
	    printf("--------------------------\n");


	    if((pid = fork()) < 0){
	        perror("fork error");
	        exit(1);
	    }
	    else if(pid == 0){
	        printf("pid: %d, ppid: %d\n", getpid(), getppid());
	        pause();  //暂停测试
	    }

	    do{
	    	//暂停测试需要用waitpid来捕获暂停的信号，并返回
	        pid = waitpid(pid, &status, WNOHANG | WUNTRACED);

	        if(pid == 0){
	            sleep(1);
	        }
	    }while(pid == 0);
	    out_status(status);

	    return 0;
	}
```

- 运行测试

	1. 运行程序

		![测试](https://raw.githubusercontent.com/McXing/Pictures/master/wait/%E6%B5%8B%E8%AF%951.png "测试")

	2. 发送信号给程序

		![测试](https://raw.githubusercontent.com/McXing/Pictures/master/wait/%E6%B5%8B%E8%AF%952.png "测试")

	3. 测试结果

		![测试](https://raw.githubusercontent.com/McXing/Pictures/master/wait/%E6%B5%8B%E8%AF%953.png "测试")
