/* stdio.c - 标准输入、标准输出、标准错误描述字 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int size;
	char buf[1024];

	printf("Before close\n");
	close(1);
	printf("After close\n");
	size = write(1, "This is standard output\n", strlen("This is standard output\n"));
	if(size < 0 ) {
		//printf("Write failed\n");
		perror("Write failed:");
		fprintf(stderr, "errno=%d, str=%s\n", errno, strerror(errno));
	}
	
	write(1, "Please input:", strlen("Please input:"));
	memset(buf, 0, 1024);
	size = read(0, buf, 1024);
	write(1, "Your input:", strlen("Your input:"));
	write(1, buf, strlen(buf));
	write(2, "This is stderror\n", strlen("This is stderror\n"));

	return 0;
}
