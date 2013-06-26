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
	int size = 0;
	char str[100];
	int a[2];
	char buf[10];
	char tmp[100];
	char *p;

	strcpy(str, "afdsjakfdjsakfjdasbcdefg");
	strcpy(tmp, "1234567");

	size = read(1, buf, 10);
	printf("Read %d bytes\n", size);
	/*
	printf("a=%d\n", a[0]);
	fd = open("./test.txt", O_CREAT|O_RDONLY, S_IRWXU);
	if(fd < 0) {
		printf("Open file test.txt failed\n");
		perror("Open file failed");
		return 1;
	}
	//size = read(fd, buf, 100);
	strcpy(buf, str);
	if(size > 0 )
		buf[size] = '\0';
	printf("Read %d bytes:%s\n", size, buf);
	*/

	size = write(a[0], "smile", 5);
	if(size < 0) {
		printf("Write to file %d failed, %s\n", a[0], strerror(errno));
	}
	printf("str=%s\n", str);
	printf("buf=%s\n", buf);
	printf("tmp=%s\n", tmp);
	
	close(fd);
	
	printf("\n");
	return 0;
}
