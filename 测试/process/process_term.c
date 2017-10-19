/*************************************************************************
	> File Name: process_term.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月09日 星期六 12时58分37秒
 ************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

//定义进程的终止函数

void term_fun1(void){
    printf("first term fucntion\n");
}

void term_fun2(void){
    printf("second term fucntion\n");
}

void term_fun3(void){
    printf("third term fucntion\n");
}

int main(int argc, char *argv[]){
    if(argc < 3){
        fprintf(stderr, "usage: %s file [exit|_exit|return]\n", argv[0]);
        exit(1);
    }
    //向内核登记终止函数
    
    atexit(term_fun1);
    atexit(term_fun2);
    atexit(term_fun3);

    FILE *fp = fopen(argv[1], "w");
    fprintf(fp, "hello iotek"); //全缓存

    if(!strcmp(argv[2], "exit")){
        exit(0);
    }
    else if(!strcmp(argv[2], "_exit")){
        _exit(0);
    }
    else if(!strcmp(argv[2], "return")){
        return 0;
    }
    else{
        fprintf(stderr, "usage: %s file [exit|_exit|return]\n", argv[0]);
    }
}
