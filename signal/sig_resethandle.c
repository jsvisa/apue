#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

//通过SA_RESETHAND，将信号的处理在处理函数入口会被重置为SIG_DF
//因此，第二次收到此信号时，不会再进入信号处理函数
//测试：向此进程发送两次SIGINT信号
void int_handler( int signum )
{
    printf("int handler %d\n",signum);
}

int main(int argc, char **argv)
{
    struct sigaction  oldact;
    struct sigaction  act;


    act.sa_handler = int_handler;
    act.sa_flags=0;
    act.sa_flags |= SA_RESETHAND;
    sigemptyset(&act.sa_mask);
    if ( -1 ==  sigaction(SIGINT, &act,&oldact) ){
        printf("sigaction failed!\n");
        return -1;
    }

    //first signal
    pause();
    printf("return from first SIGINT signal!\n");

    //second signal
    pause();
    printf("never go here!\n");

    return 0;
}
