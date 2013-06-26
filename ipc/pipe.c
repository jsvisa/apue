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
	int count;
	int size;
	pid_t pid;

	signal(SIGPIPE, func);
	memset(r_buf, 0, sizeof(r_buf));
	memset(w_buf, 0, sizeof(r_buf));
	if(pipe(pipe_fd)<0) 
	{
		printf("pipe create error\n");
		return -1;
	}

	if((pid=fork())==0) {  //child process
		close(pipe_fd[1]);
		count = 0;
		while(count < 20) {
			r_num=read(pipe_fd[0],r_buf,10000);
			if(r_num <= 0) {
				printf("read failed, return %d\n", r_num);
				break;
			}
			count += r_num;
			r_buf[r_num] = 0;
			printf("Child %d Read %d bytes :%s\n", getpid(), r_num, r_buf);
		}	
		exit(0);
	}

	count = 0;
	//parrent process
	close(pipe_fd[0]);//close read pipe
	while(1) 
	{
		if((size = write(pipe_fd[1],"hello",5)) <= 0)
		{
			perror("parent write failed\n");
			break;
		}
		else 
		{
			count += size;
			//printf("write succes:%d bytes\n", count);
		}
		sleep(1);
	}

	close(pipe_fd[1]);//close write pipe
	printf("parent close fd[1] over\n");

	exit(0);
}
