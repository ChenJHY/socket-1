/*************************************************************************
	> File Name: daemon.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月31日 星期四 13时48分47秒
 ************************************************************************/

//守护进程的简易实现原理


#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <syslog.h>

#define MAXFD 64

void daemon_init(const char *pname, int facility){
    int i;
    pid_t pid;

    /*fork 终止父进程*/
    if(pid = fork()){
        /*首先需要fork 一个子进程并将父进程关闭。如果进程是作为一个shell     命令在命令行上前台启动的，当父进程终止时，shell就认为该命令已经结束。这样子进程就自动称为了后台进程。而且，子进程从父进程那里继承了组标识符同时又拥有了自己的进程标识符，这样保证了子进程不会是一个进程组的首进程。这一点是下一步setsid 所必须的。*/
        exit(0)i;
    }

    /*第一子进程*/
    /*setsid()调用创建了一个新的进程组，调用进程成为了该进程组的首进程。这样使该进程脱离了原来的终端，成为了独立于终端外的进程*/
    setsid();

    /*忽略SIGHUP信号，重新fork，使进程不再是进程组的首进程，可以防止某些情况下进程意外打开终端而重新与终端发生联系*/
    signal(SIGHUP, SIG_IGN);
    /*fork 终止第一子进程*/

    if(pid = fork()){
        exit(0);
    }
    /*第二子进程*/

    daemon_proc = 1;

    /*将工作目录设定为"/"*/
    /*为了切断进程与原有文件系统的联系，并保证无论从什么地方启动进程都能正常工作*/
    chdir("/");

    /*清除文件掩码*/
    /*消除进程自身掩码对其创建文件的影响*/
    umask(0);

    /*关闭所有文件句柄*/
    /*防止进程继承在父进程中打开文件而使这些文件始终保持打开从而产生某些冲突*/
    for(i = 0; i < MAXFD; ++i){
        close(i);
    }

    /*打开log*/
    openlog(pname, LOG_PID, facility);
}
