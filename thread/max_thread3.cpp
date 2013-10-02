#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <bits/local_lim.h>

int globle = 0;
int k;
pthread_mutex_t lock;
pthread_cond_t cond;
void *thread_one(void * arg)
{
	usleep(500);
//	pthread_detach(pthread_self());
	k = k+1;
	printf("Thread %ld, num=%d\n", pthread_self(), k);
	pthread_mutex_lock(&lock);
	pthread_cond_signal(&cond);
	printf("Send signal...\n");
	pthread_mutex_unlock(&lock);

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
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);
	k = 0;
	i = 0;
	while(1) {
		if(k >= 300) {
			pthread_mutex_unlock(&lock);
			break;
		}
		printf("k=%d\n", k);
		if(pthread_create(&thread, &attr, thread_one, &k) != 0) {
			perror("pthread_create");
			break;
		}
//		printf("Create thread %d success\n", i++);
		pthread_mutex_lock(&lock);
		printf("Wait for signal %d...\n", i);
		pthread_cond_wait(&cond, &lock);
		pthread_mutex_unlock(&lock);
	}

	sleep(30);
//	pause();

	return 0; 
}
