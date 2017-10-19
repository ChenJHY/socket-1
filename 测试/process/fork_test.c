#include<stdio.h>
#include<unistd.h>
int g_value = 100;
int main()
{
    printf("main\n");
//    int g_value = 100;
    pid_t id = fork();
    printf("under fork\n");
    if(id == 0)
    {
        g_value = 200;
        printf("child pid=%d,father ppid=%d \n",getpid(),getppid());
        printf("child &g_value=%p\n", &g_value);
        printf("child g_value=%d\n", g_value);
    }
    else
    {
        //father
        printf("child pid=%d,father ppid=%d \n",getpid(),getppid());
        printf("father &g_value=%p\n", &g_value);
        printf("father g_value=%d\n", g_value);
    }
    //printf("&g_value=%d\n", &g_value);
    //printf("g_value = %d\n",g_value);
    wait(NULL);
    return 0;
//
}
