#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

void mySleep(int s) {
	int i;
	for(i=0; i<60; i++) {
		sleep(1);
		printf("i=%d\n", i);
	}
}

//ͨ��SA_RESTART��ʹ�����ڱ��ź��ж�ʱ�������½��뵽ϵͳ����
void int_handler( int signum )
{
    printf("int handler %d\n",signum);
}

int main(int argc, char **argv)
{
	int size;
    char buf[100];
    ssize_t ret;
    struct sigaction  oldact;
    struct sigaction  act;


    act.sa_handler = int_handler;
    act.sa_flags=0;
	//������������ע�͵����۲����н��
    act.sa_flags |= SA_RESTART;
    sigemptyset(&act.sa_mask);
    if ( -1 ==  sigaction(SIGINT, &act,&oldact) ){
        printf("sigaction failed!\n");
        return -1;
    }
	size = sleep(100);
	printf("size=%d\n", size);
	/*
	size = read(STDIN_FILENO, buf, 100);
	printf("Read %d bytes\n", size);
	if(size < 0)
		perror("read");
		*/

    return 0;
}

