#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void sighandler(int sig) {
	printf("I am thread:%d\n", pthread_self());
}

void *mThread(void *args) {
	pthread_detach(pthread_self());
    signal(SIGUSR1, sighandler);
	printf("I am thread\n");
	sleep(100);
	printf("Thread wakeup\n");
	return NULL;
}

int main(int argc, char **argv) {
	pthread_t tid;

	printf("PID:%d\n", getpid());

	pthread_create(&tid, NULL, mThread, NULL);
	sleep(100);
	printf("Main process wake\n");
	sleep(3);
	return 0;
}
