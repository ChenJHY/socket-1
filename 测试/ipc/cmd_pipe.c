/*************************************************************************
	> File Name: cmd_pipe.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月27日 星期三 16时24分33秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>

//cat /etc/passwd | grep root

char *cmd1[3] = {"/bin/cat", "/etc/passwd", NULL};
//char *cmd2[3] = {"/bin/grep", "root", NULL};
char *cmd2[3] = {"wc", "-l", NULL}; //读取文件的行数

int main(void)
{
    int fd[2];
    if(pipe(fd) < 0){
        perror("pipe error");
        exit(1);
    }

    int i = 0;
    pid_t pid;

    for(; i < 2; i++){
        pid = fork();
        if(pid < 0){
            perror("fork error");
            exit(1);
        }else if(pid == 0){
            if(i == 0){ //第一个子进程写入数据
                close(fd[0]);
                
                //将标准输出重定向到管道的写端
                //下面命令的执行结果会写入到管道中，而不是输出到屏幕
                if(dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO){
                    perror("dup2 error");
                }
                close(fd[1]);

                //调用exec函数执行cat命令
                if(execvp(cmd1[0], cmd1) < 0){
                    perror("execvp error");
                    exit(1);
                }

                break;
            }

            if(i == 1){ //第二个子进程读取数据
                close(fd[1]);   
                
                //将标准输入重定向到管道的读端
                //下面命令而不是从标准输入读取
                if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO){
                    perror("dup2 error");
                }
                close(fd[0]);

                //调用exec函数执行grep命令
                if(execvp(cmd2[0], cmd2) < 0){
                    perror("execvp error");
                    exit(1);
                }

                break;
            }
        }else{
            if(i == 1){
                //父进程等待子进程全部创建完毕回收
                close(fd[0]);
                close(fd[1]);
                wait(0);
                wait(0);
            }
        }
    }

    return 0;
}
