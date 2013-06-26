#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void *thread_two(void *arg) {
	int *fd = (int *)arg;

	write(*fd, "hello", 5); 
	return NULL;
}
void *thread_one( void * arg)
{
	int *fd = (int *)arg;

	*fd = open("./a.txt", O_RDWR|O_CREAT, S_IRWXU);
	if(*fd < 0) {
		perror("open");
		return NULL;
	}
	sleep(2);
	write(*fd, "nihao", 5); 

	return NULL;
}

int main( int arg, char ** argv)
{
	int fd;
	pthread_t thread;

	if(pthread_create( &thread, NULL, thread_one, &fd) != 0)
		perror("pthread_create");
	sleep(1);
	if(pthread_create( &thread, NULL, thread_two, &fd) != 0)
		perror("pthread_create");
	sleep(10);
	close(fd);
	return 0; 
}
