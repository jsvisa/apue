#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>

void term_handler( int signum )
{
    printf("handler %d\n",signum);
	if(signum == SIGINT)
		return;
	pause();
	printf("wake up from signal %d\n", signum);
}

int main(int argc, char **argv)
{
    char buf[100];
    ssize_t ret;
    struct sigaction  oterm;
    struct sigaction  nterm;


    nterm.sa_handler = term_handler;
//	nterm.sa_handler = SIG_IGN;//=SIG_DFL
    nterm.sa_flags=0;
    sigemptyset(&nterm.sa_mask);
	sigaddset(&nterm.sa_mask, SIGINT);
	sigaddset(&nterm.sa_mask, SIGPIPE);
    //if ( signal(SIGTERM, term_handler)){
    if ( -1 ==  sigaction(SIGTERM, &nterm,NULL) ){
        printf("sigaction failed!\n");
        return -1;
    }
    if ( -1 ==  sigaction(SIGINT, &nterm,NULL) ){
        printf("sigaction failed!\n");
        return -1;
    }

	while(1) {
		pause();
	}

    return 0;
}
