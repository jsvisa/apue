#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int flag = true;

void int_handler( int signum )
{
   printf("int handler %d\n",signum);
}

void quit_handler( int signum )
{
   flag = false;
   printf("in quit handler\n");
}

int main(int argc, char **argv)
{
   char choice;
   sigset_t mask;
   sigset_t pending;

   printf("SIGINT=%d\n", SIGINT);

   if ( signal(SIGINT, int_handler) == SIG_ERR ){
      printf("sigaction failed!\n");
      return -1;
   }

   if ( signal(SIGQUIT, quit_handler) == SIG_ERR ){
      printf("sigaction failed!\n");
      return -1;
   }


   sigemptyset(&pending);
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
   while(flag){
      pause();
      if ( - 1== sigpending(&pending) ){
         printf("sigpending failed!\n");
         return -1;
      }
      if ( sigismember( &pending, SIGINT)){
         printf("SIGINT is in pending!\n");
      }
	  else 
		  printf("No signal SIGINT received\n");
   }

   printf("before unblock\n");
   if ( -1 == sigprocmask(SIG_UNBLOCK,&mask,NULL) ){
      printf("sigprocmask failed!\n");
      return -1;
   }

   printf("after unblock\n");

   /*critical section*/
   //sleep(2);
   pause();

   return 0;
}
