#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

/*
 * 一个简单的信号处理函数，处理SIGINT信号
 */
int i = 0;
bool flag=true;

void int_handler( int num )
{
	printf("SIGNAL:%d\n", num);
	sleep(30);
	printf("In SIGINT handler %d\n", i++);
	if(i>5)
		flag = false;
}

void int_handler2(int num) {
	printf("i am in handler2\n");
}

int main(int argc, char **argv)
{
	__sighandler_t hand;
//	sighandler_t hand;
	if ( SIG_ERR == signal(SIGINT, int_handler)){
		printf("registr alarm handler failed!\n");
		return -1;
	}
	signal(SIGTERM, int_handler2);

//	 hand = signal(SIGINT, int_handler2);

//	 hand(3);
	while(flag){
		printf("start sleep...\n");
		pause();
		/*
		if(i>3) {
			signal(SIGINT, SIG_IGN);
		}
		*/
	}



	return 0;
}
