#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>


void term_handler( int num )
{
    printf("In SIGTERM handler %d\n",getpid());
}

//���Ƿֱ������ӽ��̷���SIGTERM��SIGINT�źţ����Կ����ӽ�����η�Ӧ
int main(int argc, char **argv)
{
    pid_t pid;

    if (SIG_ERR == signal( SIGTERM, term_handler)){
        printf("registr SIGTERM  handler failed!\n");
        return -1;
    }
  
    if ( SIG_ERR == signal( SIGINT, SIG_IGN)){
        printf("registr SIGINT ignore  handler failed!\n");
        return -1;
    }

    pid = fork();
    if ( pid<0 ){
        printf(" fork error!\n");
        return -1;
    }
    if ( 0 == pid ){
		while(1) {
			pause();
		}
//        execv("term_int",argv);
        printf("never go here\n");
        return -1;
    }

	while(1) {
		pause();
	}
    return 0;
}
