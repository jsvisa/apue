#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void func(int num) 
{
	printf("Caught a SIGPIPE signal\n");
}

int main(int argc, char **argv) 
{
	int i;
	int pipe_fd[2];
	char r_buf[10001];
	char w_buf[4];
	char* p_wbuf;
	int r_num;
	int cmd;
	pid_t pid;

	signal(SIGPIPE, func);
	memset(r_buf, 0, sizeof(r_buf));
	memset(w_buf, 0, sizeof(r_buf));
	if(pipe(pipe_fd)<0) 
	{
		printf("pipe create error\n");
		return -1;
	}

	for(i=0; i<10; i++) {
		if((pid=fork())==0) {  //child process
			close(pipe_fd[0]);
			sprintf(w_buf,"Child %d send\n", getpid());
			for(i=0; i<10000; i++) {
				if(write(pipe_fd[1], w_buf, strlen(w_buf)) < 0)
				{
					printf("child write failed\n");
					break;
				}
				usleep(100);
			}
			exit(0);
		}
	}

	close(pipe_fd[1]);
	while(1) 
	{
		r_num=read(pipe_fd[0],r_buf,10000);
		if(r_num > 0) {
			r_buf[r_num] = 0;
			printf("%s", r_buf);
		}
		else {
			break;
		}
	}	
	sleep(3);
	exit(0);

	//parrent process
	close(pipe_fd[0]);//close read pipe

	close(pipe_fd[0]);//close write pipe

	exit(0);
}
