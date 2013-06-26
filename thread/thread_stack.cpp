#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <bits/local_lim.h>

int globle = 0;

pthread_mutex_t lock;
void *thread_one(void * arg)
{
	char a[1024*1024*5];
	int *num;
	num = (int *)arg;
	printf("I am thread\n");
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

	p = (char *)malloc(4*1024*1024);
	pthread_attr_init(&attr);
	pthread_attr_setstack(&attr, p, 10*1024*1024);
	pthread_create(&thread, &attr, thread_one, &i);
	sleep(30);
//	pause();

	return 0; 
}
