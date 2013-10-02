#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1;
unsigned int count = 0;
//µÈ¼ÛÓÚ£ºpthread_mutex_init(&mutex1, NULL);
//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock;

void * thread_func(void *arg)
{
	char buf[200];

	printf("start lock...%ld\n", pthread_self());
	if(pthread_mutex_lock(&lock) == 0) {
		printf("lock success...%ld\n", pthread_self());
		count++;
		if(pthread_mutex_lock(&lock) == 0) {
			printf("lock success...%ld\n", pthread_self());
			pthread_mutex_unlock(&lock);
		}
		else {
			perror("lock");
		}
		sleep(3);

		pthread_mutex_unlock(&lock);
		printf("unlocked %ld\n", pthread_self());
	}
	else {
		printf("try lock failed...%ld\n", pthread_self());
	}
	return NULL;
}

int main ( int argc, char **argv)
{
	int i;

	pthread_t thread_one, thread_two;


	pthread_mutexattr_t  mutexattr;
	pthread_mutexattr_init(&mutexattr);
	pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);

	pthread_mutex_init(&lock, &mutexattr);

	for(i=0; i<3; i++) {
		if(0 != pthread_create(&thread_one, NULL, thread_func,NULL)){
			printf("pthread create failed!\n");
			return -1;
		}
	}

	getchar();
	printf("count=%u\n", count);
	pthread_mutex_destroy(&mutex1);

	return 0;
}

