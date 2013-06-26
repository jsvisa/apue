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

	fd = open("a.txt", O_RDONLY);
	if(fd < 0) {
		printf("open failed\n");
		return 0;
	}
	pid = fork();
	printf("forked, I am %d, my parent is:%d\n", (int)getpid(), (int)getppid());
	if(pid == 0) { //子进程
		/*
		while(1) {
			usleep(200000);
			printf("I am child\n");
		}
		*/

		close(fd);
		fd = open("a.txt", O_RDONLY);
		printf("in child:%d\n", fd);

		a = 100;
		read(fd, buf, 5);
		printf("Child read:%s\n", buf);
		printf("child a=%d\n", a);
		close(fd);
		printf("Child exit\n");
		exit(0);
	}
	else if(pid > 0) { //主进程
		sleep(1);
		printf("in parent:%d\n", fd);
		/*
		while(1) {
			usleep(200000);
			printf("I am parent\n");
		}
		*/
		read(fd, buf, 5);
		printf("Parent read:%s\n", buf);
		printf("parent a=%d\n", a);
		printf("this is parrent process, pid=%d\n", getpid());
	}
	else { //fork 失败
	}
	return 0;
}

