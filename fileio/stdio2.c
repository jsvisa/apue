/* stdio2.c 关闭某个标准输入输出描述字示例 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv) {
	int size;
	int fd;
	char buf[1024];


	memset(buf, 0, sizeof(buf));
	size = read(0, buf, 1000);
	printf("Read return:%d, %s\n", size, buf);
	/*
	size = write(1, "Hello\n", 6);
	if(size < 0) {
		perror("write failed");
	}
	write(2, "end...\n", 7);
	*/
	/*
	memset(buf, 0, sizeof(buf));
	printf("Please input:\n");
	size = read(STDIN_FILENO, buf, 1024);
	printf("Your input is:%s\n", buf);
	size = write(1, "this is stdout\n", strlen("this is stdout\n"));
	printf("1.size=%d\n", size);
	size = write(2, "This is stderror\n", strlen("This is stderror\n"));
	printf("2.size=%d\n", size);
	close(1);
	write(2, "PRINTF STDERROR\n", strlen("PRINTF STDERROR\n"));
	if(printf("3.size=%d\n", size) < 0) {
		perror("FPRINTF");
	}
	*/
	return 0;
}
