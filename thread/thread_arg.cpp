#include <pthread.h>
#include <unistd.h>
#include <stdio.h>


void *thread_one( void * arg)
{
	sleep(1);
	printf("this is thead one, globle=%d\n",*(int*)arg);
}

void create_thread(void )
{
	pthread_t thread;
	int i=10;

	pthread_create( &thread, NULL, thread_one, &i);
	//若sleep 2秒由结果会不一样
	sleep(2);
}

int main( int arg, char ** argv)
{
	create_thread();
	sleep(5);

	printf("primary thread exit!\n");
	return 0; 
}

