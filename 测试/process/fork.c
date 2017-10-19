/*************************************************************************
	> File Name: fork.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月30日 星期三 16时35分44秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t pid;
    printf("Only one!\n");

    pid = fork();
    if(!pid){
        printf("I am child!\n");
    }
    else if(pid > 0){
        printf("I am parent!\n");
    }
    else{
        printf("Fork error!\n");
    }

    return 0;
}
