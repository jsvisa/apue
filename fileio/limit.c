/* stdio3.c - 测试文件 打开个数*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

int main(int argc, char **argv) {
	int i;
	char name[128];
	int fds[2048];
	int fd;
	int size;
	char *buf;
	struct timeval tv;

	for(i=0; i<2048; i++) {
		sprintf(name, "%d.txt", i);
		fds[i] = open(name, O_CREAT|O_TRUNC|O_RDWR|O_APPEND, S_IRWXU);
		if(fds[i] < 0) {
			printf("Open file %s failed\n", name);
			return 1;
		}
	}
//	write(fd, "append string", strlen("append string"));
	printf("over\n");
	return 0;
}
