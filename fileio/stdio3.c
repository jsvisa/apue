/* stdio3.c - ²âÊÔO_SYNC */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i;
	int fd;
	int size;
	char *buf;
	char buffer[100];
	struct timeval tv;

	/* ÎÄ¼þ¿Õ¶´ */
	/*
	memset(buffer, 0, sizeof(buffer));
	size = read(0, buffer, 10);
	printf("read %d bytes:%s\n", size, buffer);
	size = read(0, buffer, 100);
	printf("read %d bytes:%s\n", size, buffer);


	memset(buffer, 0, sizeof(buffer));
	fd = open("./test.txt", O_CREAT|O_RDWR, S_IRWXU|S_IRWXG|S_IRWXO);
	if(fd < 0) {
		perror("open failed");
		return 0;
	}
	printf("Open success, fd=%d\n", fd);

	lseek(fd, 50, SEEK_END);
	write(fd, "lllll", 5);
	return 0;
	*/

	fd = open("./test.f", O_CREAT|O_RDWR|O_SYNC, S_IRWXU);
	if(fd < 0) {
		printf("Create file failed\n");
		return 1;
	}
	buf = (char *)malloc(1024*1024*100);
	memset(buf, 'b', 1024*1024*100);
	gettimeofday(&tv, NULL);
	printf("Start time:\t%d.%d\n", tv.tv_sec, tv.tv_usec);
	write(fd, buf, 1024*1024*100);
	gettimeofday(&tv, NULL);
	printf("End time:\t%d.%d\n", tv.tv_sec, tv.tv_usec);
	free(buf);
	close(fd);

	return 0;
}
