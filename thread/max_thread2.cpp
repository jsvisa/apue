#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <bits/local_lim.h>

int globle = 0;
int k;
void *thread_one(void * arg)
{
//	pthread_detach(pthread_self());
	k = k+1;
	printf("Thread %u, num=%d\n", pthread_self(), k);

	sleep(k);
	return NULL;
}

int main( int arg, char ** argv)
{
	pthread_t thread;
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
		printf("k=%d\n", k);
		if(pthread_create(&thread, &attr, thread_one, &k) != 0) {
			perror("pthread_create");
			break;
		}
	}

	sleep(30);
//	pause();

	return 0; 
}
