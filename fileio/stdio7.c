#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include "filelock.h"

int main(int argc, char **argv) {
	int ret;
	int fd;
	int start, end;

	if(argc != 3) {
		start = 0;
		end = 0;
	}
	else {
		start = atoi(argv[1]);
		end = atoi(argv[2]);
	}

	fd = open("./test.f", O_CREAT|O_RDWR, S_IRWXU);
	if(fd < 0) {
		printf("Create file failed:%s\n", strerror(errno));
		return 1;
	}
	ret = writeLock(fd, 20, SEEK_SET, 5);
	if(ret == 1) {
		printf("Set write lock success\n");
		getchar();
		ret = writeLock(fd, 10, SEEK_SET, 5);
		if(ret == 1) {
			printf("set lock success\n");
			unlock(fd, 10, SEEK_SET, 5);
		}
		else {
			printf("lock failed\n");
		}
			
		unlock(fd, 0, SEEK_SET, 0);
	}
	else {
		printf("Set write lock failed\n");
	}
	close(fd);
	return 0;
}
