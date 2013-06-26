#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include "filelock.h"

int fd;
void *myThread(void *arg) {
	if(writeLock(fd, 0, SEEK_SET, 0))
		printf("lock success in thread\n");
	else
		printf("lock failed in thread\n");
	exit(0);
}

int main(int argc, char **argv) {
	int i;
	int size = 1;
	char tmp[20];
	char buf[11];
	pthread_t tid;
	struct timeval tv;

	fd = open("./a.txt", O_CREAT|O_RDWR, S_IRWXU);
	if(fd < 0) {
		perror("Open file failed");
		return 1;
	}
	if(writeLocknw(fd, 0, SEEK_SET, 0)) {
		printf("Add write lock success\n");
		sprintf(buf, "ufidsa");
		size = write(fd, buf, strlen(buf));
		getchar();
	//	unlock(fd, 0, SEEK_SET, 0);
		printf("Write lock released\n");
	}
	else {
		printf("Add write lock failed\n");
	}

	close(fd);
	return 0;
}
