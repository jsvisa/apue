#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>


void kill_stop_handler( int num )
{
    printf("In SIGINT handler\n");
}


//�����޷�ΪSIGKILLע���źŴ�����
int main(int argc, char **argv)
{
    if ( SIG_ERR == signal( SIGKILL, kill_stop_handler)){
        printf("registr SIGKILL  handler failed!\n");
        return -1;
    }

    pause();
    return 0;
}
