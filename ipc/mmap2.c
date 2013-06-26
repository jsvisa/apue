#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char ** argv) {
	char *area;
	pid_t pid;
	int i;

	int fd;
	fd = open("./a.txt", O_RDWR);
	if(fd < 0) {
		printf("Open failed\n");
		return 0;
	}
	area = mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_SHARED,fd,0);
	if ( MAP_FAILED == area ){
		printf("map failed\n");
		return -1;
	}
	printf("area:%s\n", area);
	strcpy(area, "this is test");
	for(i=0; i<10; i++) {
		printf("area:%s\n", area);
		sleep(1);
	}

	munmap(area, 100);
	close(fd);
	return 0;
}
