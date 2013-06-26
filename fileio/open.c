#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int i;
	int fd, fd1;
	char buf[1024];
	int size;

	fd = open(argv[1], O_RDWR | O_APPEND| O_CREAT);
	if(fd < 0) {
		perror("Open failed");
		return 1;
	}

	lseek(fd, 0, SEEK_SET);
	size = write(fd, "hello\n", 6);
	size = write(fd, "smile\n", 6);
	if(size < 0) {
		printf("write failed:%s\n", strerror(errno));
	}
	close(fd);
	/*
	for(i=0; i<1025; i++) {
		fd = open("./a.txt", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP);
		if(fd < 0) {
			perror("Open failed");
			return 1;
		}
		printf("fd=%d\n", fd);

		size = write(fd, "fdsa", 4);
		printf("size=%d\n", size);
	}
	*/
	return 0;

	/*
	sleep(10);
	memset(buf, 0, sizeof(buf));
	size = read(fd, buf, 10);
	printf("read %d bytes:%s\n", size, buf);
	close(fd);
	return 0;
	*/
}
