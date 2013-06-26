#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
	int size;
	char buf[100];
	pid_t pid;
	int fd[2];

	printf("Main process pid is %d\n", getpid());
	pipe(fd);
	pid = fork();
	if(pid == 0) {
		pid = fork();
		if(pid == 0) {
			memset(buf, 0, sizeof(buf));
			sleep(3);
			printf("My parent is :%d\n", getppid());
			close(fd[1]);
			size = read(fd[0], buf, 100);
			printf("read %d bytes:%s\n", size, buf);
			exit(0);
		}
		exit(0);
	}
	sleep(5);
	write(fd[1], "hehe", 4);
	sleep(2);
	exit(0);
}
