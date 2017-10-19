/*************************************************************************
	> File Name: popen_rw.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月27日 星期三 20时51分36秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <memory.h>

/*popen的函数操作：
 * r：exec(cmd)子进程将命令结果写入管(标准输出重定向管道的写端)，父进程读取结果放置到FILE*的文件指针的结构提缓存中
 * w：exec(cmd)子进程从管道读取数据作为命令的输入(标准输入重定向到管道的读端)，父进程将结构体缓存中数据写入管道
 */

int main(void)
{
    FILE *fp;
    //命令执行结果放置在fp指向的结构体缓存中
    fp = popen("cat /etc/passwd", "r");

    char buf[512];
    memset(buf, 0, sizeof(buf));
    while(fgets(buf, sizeof(buf), fp) != NULL){
        printf("%s", buf);
    }
    pclose(fp);

    printf("------------------------\n");

    fp = popen("wc -l", "w");
    //向fp指向的结构体缓存中体缓存中写入数据
    fprintf(fp, "1\n2\n2\n6\n");
    pclose(fp);

    exit(0);
}
