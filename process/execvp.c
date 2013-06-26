#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

extern char **environ;

int main(int argc, char **argv)
{
    char *env_list[]={"user=jli","PATH=/:./",NULL};
    int ret;
	environ = env_list;
    ret = execvp("echoall",argv);
    if( -1==ret ){
        printf("execvp error:%s\n",strerror(errno));
        return -1;
    }

    
    printf("never go here!\n");
  
    return 0;
    
}

