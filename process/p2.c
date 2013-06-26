#include <unistd.h>

#include <stdio.h>

int main(int argc, char **argv) {
	printf("I am process 2\n");
	sleep(4);
	return 0;
}
