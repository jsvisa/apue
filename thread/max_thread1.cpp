#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <bits/local_lim.h>

int globle = 0;

//pthread_mutex_t lock;
//pthread_cond_t cond;
void *thread_one(void * arg)
{
	int *num;
	num = (int *)arg;
	//*num = *num+1;
//	pthread_detach(pthread_self());
	//pthread_mutex_lock(&lock);
	printf("Thread %u, num=%d\n", pthread_self(), *num);

	//pthread_mutex_unlock(&lock);

	sleep(*num);
	return NULL;
}

int main( int arg, char ** argv)
{
	pthread_t thread;
	int i=0;
	int b;
	globle = 0;
	char *p;
	int k;

	pthread_attr_t attr;
	size_t stackSize;

	pthread_attr_init(&attr);
	stackSize = PTHREAD_STACK_MIN * 8;
	pthread_attr_setstacksize(&attr, stackSize);
//	pthread_mutex_init(&lock, NULL);
//	pthread_cond_init(&cond, NULL);
	k = 0;
	while(1) {
		//pthread_mutex_lock(&lock);
		if(k > 300)
			break;
		k++;
		//pthread_mutex_unlock(&lock);
		printf("k=%d\n", k);
		if(pthread_create(&thread, &attr, thread_one, &k) != 0) {
			printf("k=%d\n", k);
			perror("pthread_create");
			break;
		}
	}

	sleep(30);
//	pause();

	return 0; 
}
