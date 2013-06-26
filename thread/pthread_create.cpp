#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int gNum = 0;

/*
void Test() {
	.....
	pthread_exit(NULL);
}
*/
void *thread_one( void * arg)
{
	int *a;
	char *p;
	p = (char *)arg;
	gNum++;
	strcpy(p, "This is a thread\n");	
//	exit(0);
	printf("exit thread\n");
	a = (int *)malloc(4);
	*a = 10;
	printf("a=%d\n", (int)a);
	pthread_exit(a);
}

int main( int arg, char ** argv)
{
	pthread_t thread;
	int i=0;
	int *m;

	char buf[100];

	pthread_create(&thread, NULL, thread_one, buf);

	pthread_join(thread, (void **)&m);
	printf("*m=%d\n", *m);
	printf("m=%d\n", (int)m);
	free(m);
	printf("wait for enter\n");
//	getchar();
	printf("gNum=%d\n", gNum);
	printf("buf=%s\n", buf);
	return 0; 
}
