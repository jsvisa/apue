#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int fd, fd1;
	char buf[1024];
	int size;
//	close(STDIN_FILENO);
	fd = open("./a.txt", O_CREAT|O_RDWR, S_IRUSR|S_IRGRP);
	if(fd < 0) {
		perror("Open failed");
		return 1;
	}
	printf("fd=%d\n", fd);
	return 0;
	lseek(fd, 0, SEEK_END);
	write(fd, "this is log message...\n", strlen("this is log message...\n"));

	close(fd);
	return 0;
}
