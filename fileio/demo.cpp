#include "serialio.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char **argv) {
	int fd;
	int ret;
	int size;
	fd_set fds;
	char buf[1024];
	fd = open("/dev/ttyS0", O_RDWR);
	if(fd < 0) {
		perror("Open");
		return 0;
	}
	setParams(fd, 115200, 'N', 8, 1, 0, 0);
	//setCBreakMode(fd, 0);
	while(1) {
		size = read(STDIN_FILENO, buf, 1024);
		if(size > 0) {
			buf[size] = 0;
			write(fd, buf, size);
		}
		else {
			perror("read");
		}
		size = read(fd, buf, 1024);
		if(size > 0) {
			buf[size] = 0;
			write(STDOUT_FILENO, buf, size);
		}
		else {
			perror("read");
		}
	}
}
