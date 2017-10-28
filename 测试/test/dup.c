#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int fd, copyfd;
    fd = open("test.txt", O_RDWR | O_CREAT);

    //复制fd给copyfd
    copyfd = dup(fd);

    char buf1[] = "hello ";
    char buf2[] = "world!";

    //往fd文件写入内容
    if (write(fd, buf1, sizeof(buf1)) < 0) {
        perror("write fd");
    }

    //打印出fd和copyfd的偏移量，经过上面的写操作，都变成6了
    printf("%d\n", (int)lseek(fd, 0, SEEK_CUR));
    printf("%d\n", (int)lseek(copyfd, 0, SEEK_CUR));

    //往copyfd写入内容
    if (write(copyfd, buf2, sizeof(buf2)) < 0) {
        perror("write copyfd");
    }

    printf("%d\n", (int)lseek(fd, 0, SEEK_CUR));
    printf("%d\n", (int)lseek(copyfd, 0, SEEK_CUR));

    return 0;
}
