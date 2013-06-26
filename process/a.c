#include <stdlib.h>
#include <stdio.h>

void func() {
	printf("fdsa\n");
	return ;
}
int main(int argc, char **argv) {
	int i;

	char str[10*1024*1024];
	char *p;
	i = atoi(argv[1]);
	p = (char *)malloc(i*1024*1024);
	if(p == NULL) {
		perror("malloc");
	}
	printf("Hello world\n");

	free(p);
	/*
	printf("argc=%d\n", argc);
	for(i=0; i<argc; i++) {
		printf("argv[%d]=%s\n", i, argv[i]);
	}
	*/
	exit(0);
}
