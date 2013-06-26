#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>


void int_handler( int num )
{
    printf("In SIGINT handler\n");
}


int main(int argc, char **argv)
{

	//第一次注册SIGINT的信号处理函数
    if ( SIG_ERR == signal( SIGINT, int_handler)){
        printf("registr int  handler failed!\n");
        return -1;
    }
   
    printf("register SIGINT handler succeed!\n");
    pause();


	//第二次将SIGINT的处理设为系统默认处理
    if ( SIG_ERR == signal( SIGINT, SIG_DFL)){
        printf("registr int handler failed!\n");
        return -1;
    }
    printf("register SIGINT default  handler succeed!\n");

    pause();

    return 0;
}
