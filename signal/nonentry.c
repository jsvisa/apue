#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <pwd.h>


void my_alarm(int signo)
{
   struct passwd   *rootptr;

   printf("In sig handler\n");
   rootptr = getpwnam("root");
   
   printf("pid= %d\n",getpid());
   alarm(1);
}

int main(void)
{
   struct passwd   *ptr;

   if ( SIG_ERR == signal(SIGALRM, my_alarm) ){
      printf("register SIGALRM failed!\n");
      return -1;
   }

   alarm(1);
   for ( ; ; ) {
      if ((ptr = getpwnam("fang")) == NULL){
         printf("getpwnam error\n");
         return -1;
      }

	  usleep(300);
      if (strcmp(ptr->pw_name, "fang") != 0){
         printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
         return -1;
      }
   }
}

