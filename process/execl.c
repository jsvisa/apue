#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

extern char **environ;
int main(int argc, char **argv)
{
	int ret;

	char *arg[] = {"ls", "-l", "-a", NULL};

	if ( 0==fork() ){
		getchar();
		ret = execlp("echoall","echoall", "dfsajkfds", "-a", "-m", NULL);
		printf("#############33child execl over\n");
		/*
		ret = execv("/bin/ls", arg);
		if ( -1 ==ret ){
			printf("execv failed %s\n", strerror(errno));
			return -1;
		}
		*/

		return 0;
	}
	wait(&ret);
	printf("Child exited:%d\n", ret);
	/*
	   char *env[] = {"PWD=/home/class0911/public/process", NULL};
	   printf("pid=%d\n", getpid());
	   ret = execl("/bin/ls","ls", "-la", "fdsa", NULL);
	   if ( -1 ==ret ){
	   printf("execv failed: %s\n", strerror(errno));
	   return -1;
	   }
	   return 0;
	   printf("pid=%d\n", getpid());
	   printf("\n");     
	   */
	return 0;
}

