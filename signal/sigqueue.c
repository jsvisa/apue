#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    char buf[100];
    ssize_t ret;
	char *p;
	pid_t pid;

	p = (char *)malloc(100);
	union sigval val;
//	val.sival_int = p;

	val.sival_int = 99;

	pid = (pid_t)atoi(argv[1]);
	printf("my pid is :%d\n", (int)getpid());
	printf("send signal to %d\n", (int)pid);
	//发送一个整数过去
	sigqueue(pid, SIGRTMIN+8, val);

    return 0;
}
