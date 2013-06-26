#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	printf("pid:%d, ppid:%d\n", (int)getpid(), (int)getppid());
	getchar();
	return 0;
}
