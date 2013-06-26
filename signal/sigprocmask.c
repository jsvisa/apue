#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

//测试：先向进程发送SIGINT，观察进程响应情况
//然后向进程发送SIGQUIT，观察进程响应情况
//再向进程发送SIGINT，观察进程响应情况
int flag = 1;

void int_handler( int signum )
{
	printf("int handler %d\n",signum);
}

int count = 0;
//SIGQUIT的信息处理函数
void quit_handler( int signum )
{
	flag = 0;
	count++;
	printf("count=%d before sleep\n", count);
	sleep(10);
	printf("count=%d\n", count);
}

int main(int argc, char **argv)
{
	char choice;
	sigset_t mask;

	//注册SIGINT的信息处理函数为int_handler
	if ( signal(SIGINT, int_handler) == SIG_ERR ){
		printf("sigaction failed!\n");
		return -1;
	}

	//注册SIGQUIT的信息处理函数为quit_handler
	if ( signal(SIGQUIT, quit_handler) == SIG_ERR ){
		printf("sigaction failed!\n");
		return -1;
	}

	//将SIGINT信号加到阻塞信号集字里
	sigemptyset(&mask);
	if ( -1 == sigaddset(&mask,SIGINT)){
		printf("add SIGINT to mask failed!\n");
		return -1;
	}
	if ( -1 == sigprocmask(SIG_BLOCK,&mask,NULL) ){
		printf("sigprocmask failed!\n");
		return -1;
	}
	printf("SIGINT was masked, SIGQUIT was registered!\n");

	//等待信号来唤醒
	while(flag){
		pause();
	}

	//取消信号的阻塞 ，如果有信号发生，则会被递送
	if ( -1 == sigprocmask(SIG_UNBLOCK,&mask,NULL) ){
		printf("sigprocmask failed!\n");
		return -1;
	}

	printf("SIGINT was unmasked, pauseing .........\n");
	pause();

	return 0;
}
