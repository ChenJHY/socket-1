/*************************************************************************
	> File Name: excel.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月30日 星期三 17时42分50秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    printf("Executing ls\n");

    execl("/bin/ls", "ls", "-l", NULL);

    perror("excel failed to run !\n");
    exit(1);  //#include <stdlib.h>
    //return 0;
}
