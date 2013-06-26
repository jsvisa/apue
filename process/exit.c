#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int func() {
	printf("I am func\n");
	return 0;
}


int main(int argc, char **argv) {
	getchar();
	func();
	printf("In main process\n");
	exit(258);
}
