#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int main (int argc, char **argv)
{
    int status;
	char *p;
    pid_t pid, pid1;

    pid = fork();
    if( pid<0){
        printf("fork failed\n");
        return -1;
    }

    if ( pid == 0){ //子进程一
        printf("this is child process pid=%d\n",getpid());
		char *p = NULL;
		sleep(100);
		//int i = atoi(p);
		sleep(2);
		exit(1);
    }

	printf("wait child %d to exit\n", pid);
    if ( (pid = wait(&status)) == -1 ){
        printf("wait failed!\n");
    }
	printf("Child %d return status:%d\n", pid, status);
	//pid1 = waitpid(-1, &status, 0);
	if(WIFEXITED(status)) {
		printf("child process exit normally\n");
		printf("the return status of child process is %d,%d\n", 
				WEXITSTATUS(status), pid);
	}
	else {
		printf("child process exit abnormally\n");
		if(WIFSIGNALED(status)) {
			printf("child process received a signal:%d and quit\n", WTERMSIG(status));
		}
	}
	getchar();
    return 0;
}
