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

void * wake_thread (void *arg) 
{ 
	int status; 
	sleep (hibernation); 

	status = pthread_mutex_lock (&data.mutex); 
	if (status != 0) {
		printf("[%ld]Lock mutex failed!\n", pthread_self()); 
		return NULL;
	}

	data.value = 1; 
	printf("[%ld]send cond signal...\n", pthread_self());
	status = pthread_cond_signal (&data.cond); 
//	status = pthread_cond_broadcast(&data.cond); 
	if (status != 0) {
		printf("[%ld]Signal condition failed!\n", pthread_self()); 
		return NULL;
	}
	status = pthread_mutex_unlock (&data.mutex); 
	if (status != 0) {
		printf("[%ld]Unlock mutex failed!\n", pthread_self()); 
		return NULL;
	}

	return NULL; 
}

void *threadTwo(void *args) {
	pthread_mutex_lock (&data.mutex); 
	printf("[%ld]Thread Wait for condition...\n", pthread_self());
	pthread_cond_wait(&data.cond, &data.mutex); 
	printf("[%ld]Thread received the cond signal\n", pthread_self());
	pthread_mutex_unlock (&data.mutex); 
}
int main (int argc, char *argv[]) 
{
	int status;
	pthread_t threadId; 
	struct timespec timeout; 

	if (argc > 1) 
		hibernation = atoi (argv[1]); //Ë¯Ãß

	printf("[%ld]Main process start...\n", pthread_self());

	status = pthread_create (&threadId, NULL, wake_thread, NULL); 
	status = pthread_create (&threadId, NULL, threadTwo, NULL); 
	if (status != 0) {
		printf("[%ld]Create wait thread failed!\n", pthread_self()); 
		return -1;
	}

	timeout.tv_sec = time(NULL) + 5; 
	timeout.tv_nsec = 0; 

	while (data.value == 0) { 
		status = pthread_mutex_lock (&data.mutex); 
		if (status != 0){ 
			printf("[%ld]Lock mutex failed!\n", pthread_self()); 
		}
		printf("[%ld]Main process Wait for condition...\n", pthread_self());
		status = pthread_cond_timedwait (&data.cond, &data.mutex, &timeout); 
//		status = pthread_cond_wait (&data.cond, &data.mutex); 
		printf("[%ld]Main process wakeup now\n", pthread_self());
		if (status == ETIMEDOUT) { 
			printf("[%ld]Main process Condition wait timed out.\n", pthread_self()); 
			break; 
		} 
		else if (status != 0){ 
			printf("[%ld]Main process Wait on condition failed!\n", pthread_self()); 
		}
		pthread_mutex_unlock(&data.mutex);
	}

	if (data.value != 0) {
		printf ("[%ld]Condition was signaled.\n", pthread_self()); 
	}

	status = pthread_mutex_unlock (&data.mutex); 
	if (status != 0){ 
		printf("[%ld]Unlock mutex failed!\n", pthread_self()); 
		return -1;
	}
	getchar();
	return 0; 
}

