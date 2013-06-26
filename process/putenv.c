#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void changeenv( void )
{
#ifdef LOCAL
    char env[]="TEST=set by putenv"; 
#else
    char *env="TEST=set by putenv"; 
#endif
    int ret;

    ret = putenv(env);
    //ret = setenv("TEST",env,0);
    if( 0!=ret){
        printf("put env failed!\n");
    }


    printf("env TEST=%s\n",getenv("TEST"));
}

void other(void)
{
#ifdef LOCAL
    char env[]="TEST=WRONG ENV";
#else
    char * env="TEST=WRONG ENV";
#endif



    printf("env TEST=%s\n",getenv("TEST"));
}

int main(int argc, char **argv)
{
  
    changeenv();
    other();
    printf("env TEST=%s\n",getenv("TEST"));

    return 0;
}

