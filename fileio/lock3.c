#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include "filelock.h"

int main(int argc, char **argv) {
	int i;
	int fd;
	int size = 1;
	char tmp[20];
	char buf[11];
	struct timeval tv;

	fd = open("./a.txt", O_RDWR, S_IRWXU);
	if(fd < 0) {
		perror("Open file failed");
		return 1;
	}
	if(writeLocknw(fd, 0, SEEK_SET, 0)) {
		printf("Add Write lock success\n");
		size = read(fd, buf, 10);
		getchar();
		unlock(fd, 0, SEEK_SET, 0);
		printf("Write lock released\n");
	}
	else {
		printf("Add write lock failed\n");
	}

	close(fd);
	return 0;
}
