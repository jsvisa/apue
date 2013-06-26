#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int globle;
struct TestS {
	char *p;
	int c;
};

void *thread_two(void *arg) {
	struct TestS *sp;
//	pthread_detach(pthread_self());
/*

	sp = (struct TestS*)arg;
	sp->p = (char *)malloc(100);
	strcpy(sp->p, "Hello, thread\n");
	*/
	int i;
	for(i=0; i<100; i++) {
		printf("I am child thread\n");
		sleep(1);
	}
	return NULL;
}
void *thread_one( void * arg)
{
	int i;
	int a;
	int *p;
	a = *(int *)arg;
	p = (int *)arg;
	sleep(2);
	*p = 9;
	globle  = 10;
	printf("a=%d, *p=%d\n", a, *p);
	printf("this is thead one, globle=%d\n",globle);

	return NULL;
}

int main( int arg, char ** argv)
{
	pthread_t thread;
	int i=0;
	int b;
	globle = 0;
	struct TestS s;

	if(pthread_create(&thread, NULL, thread_one, (void *)&i) != 0)
		perror("pthread_create");
	printf("threadid:%u\n", (unsigned int)thread);
	sleep(3);

	printf("i = %d, globle=%d\n", i, globle);

	return 0; 
}
