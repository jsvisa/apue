#include <pthread.h>
#include <unistd.h>
#include <stdio.h>


void *thread1( void * arg)
{
	pthread_detach(pthread_self());
	printf("this is thead one\n");

	sleep(1);
	return (void *) 1;
}


int main( int arg, char ** argv)
{
	pthread_t thread_one;
	void *ret_value;

	if ( 0!=pthread_create( &thread_one, NULL, thread1, NULL) ){
		printf("create thread one failed!\n");
		return -1;
	}

	//if the thread is terminated, this call will failed!
	//pthread_detach(thread_one);
	sleep(1);
	if(0 != pthread_join(thread_one, (void **)&ret_value)){
		printf("join thread one failed!\n");
		return -1;
	}

	printf("thread one return value is %d\n", (int)ret_value);
	return 0; 
}

