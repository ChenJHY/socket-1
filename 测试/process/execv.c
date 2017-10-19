/*************************************************************************
	> File Name: excel1.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月30日 星期三 17时50分51秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    char *av[] = {"ls", "-l", NULL};

    execv("/bin/ls", av);

    perror("execv failed!\n");

    exit(1);
}
