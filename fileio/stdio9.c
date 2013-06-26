/* stdio3.c - 测试两个进程同时写一个文件 */
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

	close(STDOUT_FILENO);
	if(argc < 2) {
		printf("Please enter a argument\n");
		return 0;
	}
	fd = open("./test.txt", O_CREAT|O_RDWR, S_IRWXU);
	sprintf(buf, "fd=%d", fd);
	if(fd < 0) {
		printf("Create file failed:%s\n", strerror(errno));
		return 1;
	}
	write(fd, buf, strlen(buf));
	size = printf("mmmfd=%d\n", fd);
	if(size <=0) {
		write(fd, "write string to file\n", 20);
		perror("Write failed\n");

		close(fd);
		return 0;
	}
	write(STDOUT_FILENO, "write success\n", strlen("write success\n"));
	printf("Hello, I am stdin\n");
//	size = write(fd, argv[1], strlen(argv[1]));
	close(fd);

	return 0;
}
