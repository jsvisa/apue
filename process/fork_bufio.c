#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char **argv)
{
    pid_t pid;

    //printf("before fork pid=%d\n",getpid());
    printf("before fork pid=%d",getpid());

    pid = fork();

    if( pid<0){
        printf("fork failed!\n");
        return -1;
    }

   if ( pid == 0 ){
        printf("This is child process! parent pid=%d,my pid=%d\n",getppid(),getpid());
   }
   else{
      printf("This is parent process! my pid=%d, child pid=%d\n", getpid(), pid);
   }
   printf("before return pid=%d\n",getpid());

   return 0;
}
