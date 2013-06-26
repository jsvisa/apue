#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *test_thread(void *arg)
{
	int i = (int)arg;
	printf("I am thread %d\n", i);
	sleep(10);
	return NULL;
}

int main( int arg, char ** argv)
{
	int i;
	pthread_t thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&attr, 1024*1024*2);

	for(i=0; i<1600; i++) 
	{
		if(pthread_create( &thread, &attr, test_thread, (void *)i) != 0) 
		{
			perror("pthread_create");
			break;
		}

		usleep(100);
	}

	getchar();

	pthread_attr_destroy(&attr);
	return 0; 
}

