#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bits/local_lim.h>

int globle;

void *thread_one( void * arg)
{
	int *i = (int *)arg;
	pthread_detach(pthread_self());
	printf("I am thread %d\n", *i);
	free(i);
	sleep(3);

	return NULL;
}

int main( int arg, char ** argv)
{
	int i;
	pthread_t thread;
	int *b;
	pthread_attr_t attr;
	size_t stackSize;

	pthread_attr_init(&attr);
	stackSize = PTHREAD_STACK_MIN * 8;
	pthread_attr_setstacksize(&attr, stackSize);


	for(i=0; i<400; i++) {
		b = (int *)malloc(4);
		*b  = i;
		if(pthread_create( &thread, &attr, thread_one, b) != 0) {
			perror("pthread_create");
			break;
		}
		usleep(300);
	}

	sleep(3);
	return 0; 
}
