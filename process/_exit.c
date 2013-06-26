#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int i = 100;
void func(void) {
	printf("atexit\n");
}
int test() {
	if(i ==100)
		exit(0);
	else
		printf("i!=100\n");
	return 0;
}
int main(int argc, char **argv) {
	atexit(func);
	printf("This is buffer io\n");
	_exit(0);
//	test();
//	printf("after test\n");
	return 0;
}
