#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if(fork() == 0) {
		system("./getpid");
		printf("I am %d\n", getpid());
		exit(0);
	}
	else {
		printf("parent process:%d\n", getpid());
	}
}
