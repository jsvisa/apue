/* stdio3.c - ≤‚ ‘O_SYNC */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

int main(int argc, char **argv) {
	int i;
	int fd;
	int size;
	char *buf;
	struct timeval tv;

//	fd = open("./test.f", O_CREAT|O_RDWR|O_APPEND);
	fd = open("./test.txt", O_CREAT|O_TRUNC|O_RDWR|O_APPEND, S_IRWXU);
	if(fd < 0) {
		perror("Open file failed");
		return 1;
	}
	write(fd, "append string", strlen("append string"));
	close(fd);

	return 0;
}
