/*************************************************************************
	> File Name: fifo_read.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月27日 星期三 21时15分34秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>

/*
 *1.只要对FIFO有适当的访问权限，FIFO可用在任何两个没有任何关系的进程间通信
 *2.本质上是内核中的一块缓存，另在文件系统中以一个特殊的设备文件(管道文件)存在
 *3.在文件系统中只有一个索引块存放文件路径，没有数据块，所有数据块存放在内核中
 *4.命名管道必须读和写同时打开，否则单独的读或者写会引发阻塞
 *5.命令mkfifo创建命名管道（内部调用mkfifo函数）
 */
 
int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("usage:%s fifo\n", argv[0]);
        exit(1);
    }
    printf("open fifi read...\n");

    //打开命名管道
    int fd = open(argv[1], O_RDONLY);
    if(fd < 0){
        perror("open error");
        exit(1);
    }else{
        printf("open file success: %d\n", fd);
    }

    //从命名管道读取数据
    char buf[512];
    memset(buf, 0, sizeof(buf));
    while(read(fd, buf, sizeof(buf)) < 0){
        perror("read error");
    }
    printf("%s\n", buf);
    close(fd);

    exit(0);
}
