#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <errno.h>
int main(int argc, char**argv) {
	int fd;
	char buf[20];
	char tmp[5];
	int size;
	fd = open("./1.txt", O_RDWR);
	if(fd < 0) {
		perror("open");
		return 0;
	}
	printf("fd=%d\n", fd);
	strcpy(buf, "fydsiafjdskajfkafkafkasioew32ru9sa");
	strncpy(tmp, "hello", 5);
	size = write(fd, tmp, strlen(tmp));
	if(size < 0) {
		perror("read");

	}
	printf("fd=%d\n", fd);
	close(fd);
	return 0;
}
