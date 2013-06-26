#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main( int argc, char **argv)
{
	int a=10;
	pid_t pid;

	printf("before fork pid=%d\n",getpid());

	pid = vfork();

	if( pid<0){
		printf("fork failed!\n");
		return -1;
	}
	if(pid > 0){
		printf("This is parent process! my pid=%d, child pid=%d, a=%d\n", getpid(), pid, a);
	}
	else if ( pid == 0 ){
		a++;
		printf("This is child process! parent pid=%d,my pid=%d,  pid=%d, a=%d\n",
				getppid(), getpid(), pid, a);
		sleep(3);
		printf("child exit\n");
		//execl("/bin/ls"...);
		exit(0);
		//return 0;
	}

	return 0;

}
