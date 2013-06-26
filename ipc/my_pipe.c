//#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>
#include<string.h>
#define MAX 1024

int main(int argc,char *argv[]) {
	int size;
	int pipe_fd[2],i=0;
	pid_t pid;
	char buf[MAX];

	printf("&buf[0]=%x, &pipe_fd[0]=%x, &size=%x, dist=%x\n", &buf[0], &pipe_fd[0], &size, (int)(&pipe_fd[0]) - (int)&buf[0]);
	/*
	if(pipe(pipe_fd)<0){
		perror("pipe");
		exit(0);
	}

	if((pid=fork())==0){
		close(pipe_fd[1]);
		while(1){
			if((size=read(pipe_fd[0],buf,MAX))<=0){
				perror("read");
				exit(0);
			}
			printf("size = %d\n", size);
			buf[size]='\0';
			printf("my pid is %d\n",getpid());
		//	printf("%s", buf);
		}

		exit(0);
	}
	if((pid=fork())==0){
		close(pipe_fd[1]);
		while(1){
			if((size=read(pipe_fd[0],buf,MAX))<=0){
				perror("read1");
				exit(0);
			}
			buf[size]='\0';
			printf("my pid is %d\n",getpid());
		//	printf("%s", buf);
		}
		exit(0);
	}
	close(pipe_fd[0]);
	while(1){
		//sleep(1);
		sprintf(buf,"how are you %d\n",++i);
		if(write(pipe_fd[1],buf,strlen(buf))<=0){
			perror("write");
			exit(0);
		}
	}
	*/
	return 0;
}
