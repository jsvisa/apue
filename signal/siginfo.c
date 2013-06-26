#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>

#if 0
siginfo_t {
	int      si_signo;  /* Signal number */
	int      si_errno;  /* An errno value */
	int      si_code;   /* Signal code */
	pid_t    si_pid;    /* Sending process ID */
	uid_t    si_uid;    /* Real user ID of sending process */
	int      si_status; /* Exit value or signal */
	clock_t  si_utime;  /* User time consumed */
	clock_t  si_stime;  /* System time consumed */
	sigval_t si_value;  /* Signal value */
	int      si_int;    /* POSIX.1b signal */
	void *   si_ptr;    /* POSIX.1b signal */
	void *   si_addr;   /* Memory location which caused fault */
	int      si_band;   /* Band event */
	int      si_fd;     /* File descriptor */
}
#endif

void sigHandler(int num, siginfo_t *info, void *ptr) {
	struct TestS *s;
	printf("signum is:%d \n", num);
	printf("signo:%d, code:%d, spid:%d, uid:%d, utime:%d, stime:%d, val:%d\n", info->si_signo, info->si_code, 
			info->si_pid, info->si_uid, info->si_utime, info->si_stime, info->si_value.sival_int);
	sleep(10);
}

int main(int argc, char **argv)
{
	char buf[100];
	ssize_t ret;
	struct sigaction  oterm;
	struct sigaction  nterm;

	printf("SIGRTMIN=%d, My pid=%d\n", SIGRTMIN, (int)getpid());

	//nterm.sa_handler = term_handler;
	memset(&nterm, 0, sizeof(nterm));
	nterm.sa_sigaction = sigHandler;
	nterm.sa_flags |= SA_SIGINFO;
	sigemptyset(&nterm.sa_mask);
	if ( -1 ==  sigaction(SIGRTMIN+8, &nterm, NULL) ){
		printf("sigaction failed!\n");
		return -1;
	}
	pause();

	return 0;
}
