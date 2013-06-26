#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

void *thread1( void * arg)
{
	printf("this is thead one\n");
	sleep(1);
	return (void *) 1;
}


int main( int arg, char ** argv)
{
	pthread_t thread_one;
	pthread_attr_t thread_attr;
	void *ret_value;

	if(0 != pthread_attr_init( &thread_attr)){
		printf("pthread attr init failed!\n");
		return -1;
	}

	if(0 != pthread_attr_setdetachstate( &thread_attr, PTHREAD_CREATE_DETACHED )){
		printf("pthread set detach state failed!\n");
		return -1;
	}

	if(0 != pthread_create( &thread_one, &thread_attr, thread1, NULL)){
		printf("create thread one failed!\n");
		return -1;
	}

	pthread_attr_destroy( &thread_attr);

	if(0 != pthread_join(thread_one, (void **)&ret_value)){
		perror("pthread_joine");
		return -1;
	}

	printf("thread one return value is %d\n", (int)ret_value);

	return 0; 
}

