#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char **argv) {
	pid_t pid;
	pid = (pid_t)atoi(argv[1]);
	kill(pid, SIGTERM);
	return 0;
}
