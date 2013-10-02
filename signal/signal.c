#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

int i = 0;
//SIGALARM�źŵĴ�����
void alarm_handler( int num )
{
    printf("In SIGALRM handler:%d\n", i);
	sleep(10);
    printf("Start to sleep...\n");
}

//SIGINT�źŵĴ�����
void int_handler(int num) {
	if(num == SIGINT) {
		printf("You have pressed Ctrl+c\n");
	}
	else if(num == SIGTERM) {
		printf("Process received SIGTERM\n");
		//exit(0);
	}
	sleep(10);
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

	//��2��Ϊ30��Ȼ��ִ�д˳��򣬲���30����ʹ��kill
	//ָ����˽��̷���SIGALRM�źţ�����ִ�н��
	//alarm(6);
	for(i=0; i<50; i++) {
		printf("i=%d\n", i);
		sleep(1);
	}

    start = time(NULL);

//    pause();

    printf("after %d seconds!\n", time(NULL) - start);
    return 0;
}

