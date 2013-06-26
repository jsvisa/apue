#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	char *path = getenv("PWD");
	printf("PWD:%s\n", path);
	return 0;
}
