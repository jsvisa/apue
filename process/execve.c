#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    char *env_list[]={"usermxx=jli","PATH=./",NULL};
    pid_t pid;

    execve("echoall",argv,env_list);

    
    printf("never go here!\n");
  
    return 0;
    
}

