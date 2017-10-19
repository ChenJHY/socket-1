#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//进程链
int main(int argc, char const *argv[])
{
	int counter = 0;
	if(argc < 2){
		counter = 2;
	}
	else{
		counter = atoi(argv[1]);
	}

	int i = 1;
	pid_t pid;
	for(; i < counter; i++){
		pid = fork();
		if(pid < 0){
			perror("fork error");
			exit(1);
		}
		else if(pid > 0){
			break;	//父进程退出，子进程继续循环，
		}

		printf("pid: %d, ppid: %d\n", getpid(), getppid());
		sleep(2);
	}

	return 0;
}