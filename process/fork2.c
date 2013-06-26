#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int a=10;

int main( int argc, char **argv)
{
	
	int fd;
    pid_t pid;
	char buf[10];
	buf[5] = 0;
	int status;

	while(1) {
		pid = fork();
		if(pid == 0) { //子进程
			while(1){
				printf("This is child\n");
				sleep(1);
			}
		}
		else if(pid > 0) { //主进程
			printf("Wait child to quit...\n");
			//pid = wait(&status);
			
			printf("Child process %d exit\n", pid);
			sleep(100);
		}
		else { //fork 失败
		}
	}
	return 0;
}

