#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

//1. ��ʹ�ô˺�������FIFO
int main(int argc, char **argv) {
	if(mkfifo("./fifo.tmp", S_IRUSR | S_IWUSR) == -1) {
		printf("Create fifo failed:%s\n", strerror(errno));
	}
	return 0;
}
