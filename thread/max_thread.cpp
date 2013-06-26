#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <bits/local_lim.h>

int globle = 0;

void *thread_one(void * arg)
{
	int *num;
	num = (int *)arg;
//	printf("Thread %u, num=%d\n", pthread_self(), *num);

	sleep(*num+5);
	return NULL;
}

int main( int arg, char ** argv)
{
	int k;
	int i=0;
	int b;
	globle = 0;
	char *p;

	pthread_t thread;
	pthread_attr_t attr;
	size_t stackSize;

	pthread_attr_init(&attr);
	stackSize = PTHREAD_STACK_MIN * 100;
	printf("stackSize:%d\n", stackSize);
	pthread_attr_setstacksize(&attr, stackSize);
	k = 0;
	while(1) {
		k++;
		printf("k=%d\n", k);
		if(pthread_create(&thread, &attr, thread_one, &k) != 0) {
//		if(pthread_create(&thread, NULL, thread_one, &k) != 0) {
			perror("pthread_create");
			break;
		}

		/*
		if(pthread_create(&thread, NULL, thread_one, &k) != 0) {
			perror("pthread_create");
			break;
		}
		*/
	}

	sleep(30);
//	pause();

	return 0; 
}
