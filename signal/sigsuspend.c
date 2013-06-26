#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int flag = true;

void int_handler( int signum )
{
	printf("int handler %d\n",signum);
}

void quit_handler( int signum )
{
	flag = false;
	printf("in quit handler\n");
}

int main(int argc, char **argv)
{
	char choice;
	sigset_t mask;
	sigset_t pending;

	if (signal(SIGINT, int_handler) == SIG_ERR ){
		printf("sigaction failed!\n");
		return -1;
	}

	if ( signal(SIGQUIT, quit_handler) == SIG_ERR ){
		printf("sigaction failed!\n");
		return -1;
	}


	sigemptyset(&mask);
	if ( -1 == sigaddset(&mask,SIGINT)){
		printf("add SIGINT to mask failed!\n");
		return -1;
	}
	sigsuspend(&mask);
	printf("over\n");

	return 0;
}
