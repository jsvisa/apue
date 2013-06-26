#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char ** argv) {
	char *area;
	pid_t pid;
	int i;

	int fd;
	fd = open("./a.txt", O_RDWR);
	if(fd < 0) {
		perror("open");
		return 0;
	}
	area = mmap(NULL, 300, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//area = mmap(NULL, 300, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED,-1,0);
	if ( MAP_FAILED == area ){
		printf("map failed\n");
		close(fd);
		return -1;
	}
	printf("area=%s\n", area);
	//strncpy(area, "hello, this is mmap", strlen("hello, this is mmap"));

	pid = fork();
	if( pid < 0 ){
		printf("fork error\n");
		return -1;
	}

	if ( pid == 0){
		for(i=0; i<10; i++){
			area[i] = 'y';
		}
		sleep(1);
		munmap(area, 300);
	}
	else{ //parrent
		for(i=10; i<299; i++){
			area[i] = 'p';
		}
		area[299] = 0;
		printf("Area = %s\n", area);
		munmap(area, 300);
	}
	munmap(area, 300);
	return 0;
}

