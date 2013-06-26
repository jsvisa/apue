/* stdio5.c - lseek */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int fd;
	int pos;
	char buf[11];
	char *p;
	int size;

	fd = open("./test.f", O_CREAT|O_NONBLOCK|O_RDWR, S_IRWXU);
	if(fd < 0) {
		printf("Create file failed:%s\n", strerror(errno));
		return 1;
	}
	pos = lseek(fd, 10, SEEK_CUR);
	printf("pos=%d\n", pos);
	p = malloc(1024*1024*10);
	memset(p, '1', 1024*1024*10);
	size = write(fd, p, 1024*1024*10);
	printf("size=%d\n", size);
	free(p);
	/*
	memset(buf, 0, 11);
	read(fd, buf, 10);
	printf("Read:%s\n", buf);

	pos = lseek(fd, 10, SEEK_CUR);
	if(pos == -1) {
		printf("lseek error:%s\n", strerror(errno));
	}
	printf("Position:%d\n", pos);
	memset(buf, 0, 11);
	read(fd, buf, 10);
	printf("Read:%s\n", buf);
	*/
	close(fd);

	return 0;
}
