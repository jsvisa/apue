#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

int i = 0;
void alarm_handler( int num )
{
	i++;
    printf("In SIGALRM handler:%d\n", i);
	printf("start sleep %d\n", i);
	sleep(60);
	printf("wake up...%d\n" , i);
}

void int_handler(int num) {
	if(num == SIGINT) {
		printf("You have pressed Ctrl+c\n");
	}
	else if(num == SIGTERM) {
		printf("Process received SIGTERM\n");
		exit(0);
	}
}

int main(int argc, char **argv)
{
    __sighandler_t ret;
    time_t  start;

    ret = signal( SIGALRM, alarm_handler);
    if ( SIG_ERR == ret ){
        printf("registr alarm handler failed!\n");
        return -1;
    }
    ret = signal(SIGINT, int_handler);
    if ( SIG_ERR == ret ){
        printf("registr alarm handler failed!\n");
        return -1;
    }
    ret = signal(SIGTERM, int_handler);
    if ( SIG_ERR == ret ){
        printf("registr alarm handler failed!\n");
        return -1;
    }

    //将2改为30，然后执行此程序，并在30秒内使用kill
    //指令向此进程发送SIGALRM信号，看看执行结果
    alarm(6);
    /*
    for(i=0; i<50; i++) {
      printf("i=%d\n", i);
      sleep(1);
    }
    */
    while(1) {
      pause();
    }

    start = time(NULL);

    //pause();

    printf("after %ld seconds!\n", time(NULL) - start);
    return 0;
}
