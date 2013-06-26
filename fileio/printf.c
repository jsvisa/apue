#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char **argv) {
	int fd;
	int i;

	printf("hello\n");
	sleep(2);
	printf("hello");
	sleep(10);
	return 0;
}
