#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

//在makefifo之后调用此程序
int main(int argc, char **argv) {
	int fd;
	int size;
	char buf[1025];
	fd = open("./fifo.tmp", O_RDONLY);
	if(fd < 0) {
		printf("Open file failed:%s\n", strerror(errno));
		return 0;
	}
	printf("Open read side success\n");
	while(1) {
		size = read(fd, buf, sizeof(buf)-1);
		if(size > 0) {
			buf[size] = 0;
			printf("Read:%s\n", buf);
		}
		else {
			perror("Read fail");
			printf("errno:%d\n", errno);
			break;
		}
	}
	close(fd);
	return 0;
}
