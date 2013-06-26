#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <bits/local_lim.h>

int globle = 0;
void *thread_one(void * arg)
{
	int *num;
	num = (int *)arg;
//	pthread_detach(pthread_self());
	//pthread_mutex_lock(&lock);
	printf("Thread %u, num=%d\n", pthread_self(), *num);
	//pthread_mutex_unlock(&lock);
	sleep(*num);
	free(num);
	return NULL;
}

int newThread(int n){
	int *b;
	pthread_t thread;

	b = (int *)malloc(4);
	memcpy(b, &n, 4);
	if(pthread_create(&thread, NULL, thread_one, b) != 0) {
		perror("pthread_create");
		return -1;
	}
	return 0;
}

int main( int arg, char ** argv)
{
	int k;
	int i=0;
	int b;
	globle = 0;
	char *p;

	pthread_attr_t attr;
	size_t stackSize;

	pthread_attr_init(&attr);
	stackSize = PTHREAD_STACK_MIN * 8;
	pthread_attr_setstacksize(&attr, stackSize);
//	pthread_mutex_init(&lock, NULL);
//	pthread_cond_init(&cond, NULL);
	k = 0;
	while(1) {
		if(k >= 300) {
			break;
		}
		k++;
		printf("k=%d\n", k);
		newThread(k);

	}

	sleep(30);
//	pause();

	return 0; 
}
