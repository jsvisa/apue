#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int fd, fd1;
	char buf[1024];
	int size;
//	close(STDIN_FILENO);
	fd = open("./a.txt", O_CREAT|O_RDWR,S_IRWXU);
	if(fd < 0) {
		perror("Open failed");
		return 1;
	}
	memset(buf, 0, sizeof(buf));
	size = write(fd, "fdjksafdsjka", strlen("fdjksafdsjka"));
	sleep(10);
	close(fd);
	return 0;
}
