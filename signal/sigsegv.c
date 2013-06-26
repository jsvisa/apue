#include <signal.h>
#include <stdio.h>

void func(int num) {
	printf("recved sig %d\n", num);
}
int main(int argc, char **argv) {
	int a = 10;

	signal(SIGSEGV, SIG_IGN);
	printf("a=%s\n", a);
	printf("a=%d\n", a);
	return 0;
}
