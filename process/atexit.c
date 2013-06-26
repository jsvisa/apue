#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char *buf;
void  exit_handler(void){
	printf("I'm in exit handler\n");
	printf("Free buf\n");
	if(buf != NULL) {
		free(buf);
		buf = NULL;
	}
}
void  func() {
	int fd;
	fd = open("./test.txt", O_RDONLY);
	if(fd < 0) {
		printf("Open file failed\n");
		_exit(0);
	}
	close(fd);
}
int main(int argc, char **argv){
	buf = NULL;
	int i;
	if (0 != atexit(exit_handler)){
		printf("register exit function handler failed!\n");
		return -1;
	}
	printf("Regist success\n");
	buf = (char *)malloc(1024);
//	func();
	printf("before exit\n");

	exit(0);

// _exit(0);  return 0;
}
