#include <stdio.h>
#include <string.h>

struct tests {
	int i;
	int j;
	char *p;
	char str[10];
};

int main(int argc, char **argv) {
	struct tests a, b;
	a.i  = 10;
	a.j = 100;
	a.p = (char *)malloc(100);
	strcpy(a.p, "string");
	strcpy(a.str, "hello");
	b = a;
	strcpy(a.p, "abcde");
	strcpy(a.str, "12345");
	free(a.p);
	printf("ok\n");
	return 0;
}

