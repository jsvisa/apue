#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv) {
	char buf[1024];
	int size;
	int res;
	fd_set readFDs;
	struct timeval waitTime;

	write(STDOUT_FILENO, "Please Input:", strlen("Please Input:"));

	
	waitTime.tv_sec = 10;
	waitTime.tv_usec = 0;
	FD_ZERO(&readFDs);
	FD_SET(STDIN_FILENO, &readFDs);

	res = select(STDIN_FILENO+1, &readFDs, NULL, NULL, &waitTime);
	printf("select over\n");
	if(res <= 0 && !FD_ISSET(STDIN_FILENO, &readFDs)) {
		printf("No input\n");
		return 0;
	}

	if(FD_ISSET(STDIN_FILENO, &readFDs)) {
		size = read(STDIN_FILENO, buf, 1024);
		printf("Read %d bytes\n", size);
		if(size > 0) {
			buf[size] = 0;
			printf("Read string:%s\n", buf);
		}
	}
	return 0;
}

