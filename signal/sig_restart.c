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
//通过SA_RESTART，使进程在被信号中断时，可重新进入到系统调用
void int_handler( int signum )
{
    printf("int handler %d\n",signum);
	sleep(1);
	printf("wake up now\n");
}

int main(int argc, char **argv)
{
    char buf[100];
    ssize_t ret;
    struct sigaction  oldact;
    struct sigaction  act;


    act.sa_handler = int_handler;
    act.sa_flags=0;
	//若将下面这行注释掉，观察运行结果

    act.sa_flags |= SA_RESTART;
    sigemptyset(&act.sa_mask);
	//sigaddset(&act.sa_mask, SIGUSR1);

    if ( -1 ==  sigaction(SIGINT, &act,&oldact) ){
        printf("sigaction failed!\n");
        return -1;
    }

    bzero(buf,100);
	printf("Start read\n");
//	mySleep(60);
    ret = read(STDIN_FILENO, buf, 99);    
    if ( ret == -1 ){
        printf("ret = %d, read error %s\n", ret, strerror(errno));
        return -1;
    }
	buf[ret] = 0;
    printf("read %d bytes, content is %s\n",ret, buf); 

    return 0;
}
