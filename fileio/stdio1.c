/* stdio1.c - ������������ͬʱдһ���ļ� */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv) {
	int i;
	int fd;
	int size;
	struct timeval tv;
	char buf[128];

	fd = open("./test.txt", O_CREAT|O_RDWR|O_APPEND, S_IRWXU);
	if(fd < 0) {
		perror("Open failed");
		return 1;
	}
	for(i=0; i<500000; i++) {
//		lseek(fd, 0, SEEK_END);
		write(fd, "abcde", 5);
	}
	close(fd);
	printf("over!\n");
	return 0;
}
