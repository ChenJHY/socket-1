/*************************************************************************
	> File Name: process_id.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月09日 星期六 18时15分02秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    printf("pid: %d\n", getpid());
    printf("ppid: %d\n", getppid());
    printf("uid: %d\n", getuid()); 
    printf("euid: %d\n", geteuid()); 
    printf("user gid: %d\n", getgid()); //用户组id
    printf("gid: %d\n", getpgrp());  //当前进程所在进程组的id
    printf("pgid: %d\n", getpgid(getpid()));
    printf("ppgid: %d\n", getpgid(getppid()));

    return 0;
}
