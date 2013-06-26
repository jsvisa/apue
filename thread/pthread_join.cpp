#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void test() {
	printf("Test pthread_exit\n");
	pthread_exit(NULL);
}

void *thread1( void * arg)
{
	printf("this is thead one\n");
	sleep(5);
	return (void *)1;
}

void *thread2( void * arg)
{
	int i=10;
	//int *p = (int *)malloc(sizeof(int));

//	char p[10];
	char *p;
	p = (char *)malloc(10);
	strcpy(p, "abcde");
	test();
	sleep(4);
	printf("this is thead two\n");
	return (void *)p;
	//return (void *) &i;
}

int main( int arg, char ** argv)
{
	pthread_t thread_one;
	pthread_t thread_two;
	int *ret_value;
	char *str;

	if (0 != pthread_create( &thread_one, NULL, thread1, NULL)){
		printf("create thread one failed!\n");
		return -1;
	}
	if (0 != pthread_create( &thread_two, NULL, thread2, NULL)){
		printf("create thread two failed!\n");
		return -1;
	}

	printf("wait for thread_two to exit...\n");
	if (0 != pthread_join(thread_two, (void **)&str) ){
		printf("join thread two failed!\n");
		return -1;
	}

	sleep(2);
	printf("thread two return value is %s\n", str);
	free(str);
	printf("wait for thread_one to exit...\n");

	if (0 != pthread_join(thread_one, (void **)&ret_value) ){
		printf("join thread one failed!\n");
		return -1;
	}

	printf("thread one return value is %d\n", (int)ret_value);

	return 0; 
}

