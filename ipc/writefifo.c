#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

void pipeHandler(int num) {
	printf("Receive SIGPIPE\n");
}

int main(int argc, char **argv) {
	int fd;
	int size;
	char buf[1025];

	char *p = "a132454396096i054\n";

	signal(SIGPIPE, pipeHandler);
	fd = open("./fifo.tmp", O_WRONLY);
	if(fd < 0) {
		printf("Open file failed:%s\n", strerror(errno));
		return 0;
	}
	printf("Open write end success\n");
	//size = write(fd, p, strlen(p));
	for(;;) {
		size = write(fd, p, strlen(p));
		if(size <= 0) {
			printf("write failed:%s\n", strerror(errno));
		}
		printf("Write ok\n");
		sleep(1);
	}
	close(fd);
	return 0;
}
