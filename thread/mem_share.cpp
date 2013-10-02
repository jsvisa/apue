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
	pthread_detach(pthread_self());
	pthread_mutex_lock(&lock);
	pthread_cond_wait(&cond, &lock);
	pthread_mutex_unlock(&lock);

	printf("Thread %ld start...\n", pthread_self());
	usleep(10000);
	
	while(1) {
		pthread_mutex_lock(&lock);
		if(k<300000000) {
			k++;
			pthread_mutex_unlock(&lock);
		}
		else  {
			pthread_mutex_unlock(&lock);
			break;
		}
	}
	printf("Thread %ld, k=%d\n", pthread_self(), k);

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

	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);
	k = 0;
	i = 0;
	for(i=0; i<5; i++) {
		if(pthread_create(&thread, &attr, thread_one, NULL) != 0) {
			perror("pthread_create");
			break;
		}
//		printf("Create thread %d success\n", i++);
	}

	printf("wait...\n");
	sleep(3);
	pthread_mutex_lock(&lock);
	printf("Send signal %d...\n", i);
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&lock);

	getchar();
	printf("k=%d\n", k);
	//sleep(30);
//	pause();

	return 0; 
}
