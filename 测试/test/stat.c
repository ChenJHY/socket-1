/*************************************************************************
	> File Name: stat.c
	> Author: 
	> Mail: 
	> Created Time: 2017年10月24日 星期二 21时45分06秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(void)
{
    struct stat buf;
    stat("/etc/hosts",&buf);
    printf("stat: %d\n", buf.st_size);
    
    int filefd = open("test.txt", O_RDONLY);
    fstat(filefd, &buf);
    printf("fstat: %d\n", buf.st_size);
    
    lstat("/etc/hosts", &buf);
    printf("lstat: %d\n", buf.st_size);
    
    return 0;
}
