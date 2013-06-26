#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>


void int_handler( int signum )
{
   printf("int handler %d\n",signum);
}

int main(int argc, char **argv)
{
   char choice;


   if ( signal(SIGINT, int_handler) == SIG_ERR ){
      printf("sigaction failed!\n");
      return -1;
   }

   printf("enter k for kill , or r for raise...\n");

   while(true){
      choice = getchar();
      if ( 'r' == choice ){
         printf("raise SIGINT\n");
         raise(SIGINT);
         break;
      }
      else if ('k' == choice ){
         printf("kill SIGINT\n");
         kill(getpid(),SIGINT);
         break;
      }
      else{
         printf("you enter %c is not correct!\n",choice);
      }
   }

   return 0;
}
