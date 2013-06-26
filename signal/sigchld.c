#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

/* 检查子进程退出时父进程对SIGCHLD信号的处理 */

void chld_handler( int num ) {
    printf("In SIGCHLD handler\n");
}


int main(int argc, char **argv) {
	pid_t pid;
    __sighandler_t ret;

    ret = signal(SIGCHLD, SIG_IGN);

    if ( SIG_ERR == ret){
        printf("registr alarm handler failed!\n");
        return -1;
    }

	pid = fork();

	if(pid > 0) {
		pause();
		pause();
	}
	else {
		pause();
		exit(0);
	}
    return 0;
}
