#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

//ͨ��SA_RESETHAND�����źŵĴ����ڴ�������ڻᱻ����ΪSIG_DF
//��ˣ��ڶ����յ����ź�ʱ�������ٽ����źŴ�����
//���ԣ���˽��̷�������SIGINT�ź�
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
