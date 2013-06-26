#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
	int fd_pipe[2];
	char r_buf[100];
	char w_buf[100];
	int size;
	int i;
	int n = 0;

	pipe(fd_pipe);
	
	for (i = 0; i < 5; i++)
	{
		if (fork() == 0)
		{
			close(fd_pipe[1]);
			if ((size = read(fd_pipe[0], r_buf, 100)) <= 0)
			{
				printf("process %d read error.\n",getpid());
				close(fd_pipe[0]);
				exit(1);
			}
			r_buf[size] = 0;
			printf("process %d read: %s\n", getpid(), r_buf);
			close(fd_pipe[0]);
			return 0;
		}
		//sleep(1);
	}
	
	close(fd_pipe[0]);
	
	while (1)
	{
		sprintf(w_buf, "Hello %d", n++);
		write(fd_pipe[1], w_buf, strlen(w_buf));
		sleep(1);
	}

	close(fd_pipe[1]);
	return 0;
}
