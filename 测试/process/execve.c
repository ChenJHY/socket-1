/*************************************************************************
	> File Name: execve.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月31日 星期四 11时21分40秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>

int main(){
    char *argv[] = {"showenv", NULL},
        *envp[] = {"foo = bar", "bar = foo", NULL};

    execve("./a.out", argv, envp);
    perror("execv failed!\n");

    return 0;
}
