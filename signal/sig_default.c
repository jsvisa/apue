#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

/* 检测进程SIGINT信号默认的屏蔽信号与标志 */
//注：SIGINT 的值为2
void printMask( sigset_t sig_mask)
{
   int i;

   for(i=0;i<64; i++){
      if ( sigismember(&sig_mask,i) ){
         printf("sig %d is masked!\n",i);
      }
   }
}

void printFlags( int flags )
{
   if ( flags &SA_NOCLDSTOP){
      printf(" SA_NOCLDSTOP in flags!\n");
   }
   if ( flags & SA_RESTART ) {
      printf("SA_RESTART in flags!\n");
   }
   if ( flags & SA_ONSTACK ){
      printf(" SA_ONSTACK in flags!\n");
   }
   if ( flags & SA_NOCLDWAIT ){
      printf(" SA_NOCLDWAIT in flags!\n");
   }
   if (  flags & SA_NODEFER ){
      printf("SA_NODEFER in flags!\n");
   }
   if ( flags & SA_RESETHAND){
      printf("SA_RESETHAND in flags!\n");
   }
   if ( flags & SA_SIGINFO ){
      printf("SA_SIGINFO in flags!\n");
   }
}

void int_handler( int signum )
{
    printf("Signal :%d\n",signum);
}

int main(int argc, char **argv)
{
    char buf[100];
    ssize_t ret;
    struct sigaction  oldact;


    if ( signal(SIGINT, int_handler) == SIG_ERR ){
        printf("sigaction failed!\n");
        return -1;
    }
     if ( signal(SIGUSR1, int_handler) == SIG_ERR ){
        printf("sigaction failed!\n");
        return -1;
    }
    
    bzero( &oldact, sizeof(oldact));
    sigemptyset(&oldact.sa_mask);
    if ( -1 ==  sigaction(SIGINT, NULL,&oldact) ){
        printf("sigaction failed!\n");
        return -1;
    }

    printMask(oldact.sa_mask);
    printFlags(oldact.sa_flags);
	//检测SIGUSR1
    if ( -1 ==  sigaction(SIGUSR1, NULL,&oldact) ){
        printf("sigaction failed!\n");
        return -1;
    }

    printMask(oldact.sa_mask);
    printFlags(oldact.sa_flags);


    return 0;
}
