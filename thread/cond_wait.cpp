#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct my_struct_tag { 
	pthread_mutex_t mutex; 
	pthread_cond_t cond; 
	int value; 
} my_struct_t; 

my_struct_t data = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0}; 

int hibernation = 1; 

void * wait_thread (void *arg) 
{ 
	int val;
   val = *(int *)arg;	
	pthread_mutex_lock (&data.mutex); 
	printf("I am %d, start wait....\n", val);
	pthread_cond_wait(&data.cond, &data.mutex); 
	printf("I am %d wake up\n", val);
	pthread_mutex_unlock (&data.mutex); 

	return NULL; 
}

int main (int argc, char *argv[]) 
{
	int i;
	int status;
	pthread_t wait_thread_id; 
	struct timespec timeout; 

	if (argc > 1) 
		hibernation = atoi (argv[1]); 

	for(i=0; i<3; i++) {
		status = pthread_create (&wait_thread_id, NULL, wait_thread, &i); 
		if (status != 0) {
			printf( "Create wait thread failed!\n"); 
			return -1;
		}
		usleep(100);
	}

	for(i=0; i<3; i++) {
		printf("Main send cond signal...\n");
		pthread_mutex_lock (&data.mutex); 
		pthread_cond_signal(&data.cond);
		status = pthread_mutex_unlock (&data.mutex); 
		sleep(1);
	}
	sleep(3);
	/*
	pthread_mutex_lock (&data.mutex); 
	printf("Send broadcast...\n");
//	pthread_cond_signal(&data.cond);
	pthread_cond_broadcast(&data.cond);
	pthread_mutex_unlock (&data.mutex); 
	*/

	sleep(50);
	return 0; 
}

