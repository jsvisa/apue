#include <unistd.h>
#include <stdio.h>
int main(int argc, char **argv) {
	int i;
	pid_t pid;

	pid = fork();
	if(pid == 0) {
		for(i=0; i<100000000; i++) {
			printf("child i=%d\n", i);
			usleep(300000);
		}
		printf("I am child process, my parrent is %d\n", getppid());
		exit(0);
	}
	else if(pid > 0) {
		for(i=0; i<100000000; i++) {
			usleep(300000);
			printf("parrent i=%d\n", i);
		}
		printf("I am parrent process, my child is %d\n", pid);
	}
	printf("I am %d, Wait a minute...\n", getpid());
	sleep(30);
	return 0;
}

