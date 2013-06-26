#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char *path;
    int ret;
    int rewrite;
  
    if( 2!=argc ){
        printf("please input rewrite value! \n");
        return -1;
    }

    rewrite = atoi(argv[1]);
  
    path = getenv("TEST");
    if( NULL == path ){
		printf("env variable TEST doesn't exit!\n");
    }

    ret = setenv("TEST","this is test",0);
    if ( -1 == ret ){
        printf("set evn variable failed!\n");
		goto ERR_QUIT;
    }

    printf("TEST env variable exit!TEST=%s\n", getenv("TEST") );
    printf("call setenv with rewrite=%d\n",rewrite);

    ret = setenv("TEST","change test env",rewrite);
    if ( -1 == ret ){
        printf("set evn variable failed!\n");
		goto ERR_QUIT;
    }
    printf("TEST env variable exit!TEST=%s\n", getenv("TEST") );
ERR_QUIT:
    return 0;
}

