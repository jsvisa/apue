#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {
	int size;
	char buf[101];
	int val;
	printf("Please input:\n");

	/*
	fcntl(0, F_GETFL, &val);
	val |= O_NONBLOCK;
	fcntl(0, F_SETFL, val);
	*/

	while(1) {
		size = read(0, buf, 100);
		if(size > 0) {
			buf[size] = 0;
			printf("Your input is %s\n", buf);
		}
		else {
			printf("Read nothing, errno:%d, str:%s\n", (int)errno, strerror(errno));
			sleep(1);
		}
	}
}
