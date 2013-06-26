#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int globle;
void *test_thread(void *arg) {
	int i;
	for(i=0; i<100; i++) {
		globle = i;
		sleep(1);
	}
	return NULL;
}
void *thread_two( void * arg)
{
	int i;
	for(i=0; i<100; i++) {
		printf("In thread_two globle=%d\n", globle);
		sleep(1);
	}

	return NULL;
}

int main( int arg, char ** argv)
{
	pthread_t thread;
	if(pthread_create( &thread, NULL, test_thread, NULL) != 0) {
			perror("pthread_create");
	}
	if(pthread_create( &thread, NULL, thread_two, NULL) != 0) {
			perror("pthread_create");
	}
	getchar();
	printf("globle=%d\n", globle);

	return 0; 
}
