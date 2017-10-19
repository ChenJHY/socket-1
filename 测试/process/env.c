/*************************************************************************
	> File Name: env.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月31日 星期四 11时07分34秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>

extern char **environ;

int main(){
    char **env = environ;

    while(*env){
        printf("%s\n", *env++);
    }

    return 0;
}
