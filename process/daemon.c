#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	pid_t pid, pid1, pid2;
	int status;
	int runProc1 = 1;
	int runProc2 = 1;

	while(1) {
		if(runProc1 == 1) {
			pid1 = fork();
			if(pid1 == 0) {
				if(execl("./p1", NULL) == -1) {
					perror("Run process p1 failed");
					exit(0);
				}
			}
			runProc1 = 0;
		}
		if(runProc2 == 1) {
			pid2 = fork();
			if(pid2 == 0) {
				if(execl("./p2", NULL) == -1) {
					perror("Run process p2 failed");
					sleep(3);
					exit(0);
				}
			}
			runProc2 = 0;
		}
		pid = wait(&status);
		if(pid == pid1) {
			runProc1 = 1;
		}
		else if(pid == pid2) {
			runProc2 = 1;
		}
	}
}
