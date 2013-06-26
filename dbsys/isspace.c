#include <ctype.h>

#include <stdio.h>

int main(int argc, char **argv) {
	printf("isspace(0) = %d\n", isspace(0));
	printf("isspace('\\n') = %d\n", isspace('\n'));
	printf("isspace(' ') = %d\n", isspace(' '));
	printf("isspace('TABSPACE') = %d\n", isspace('	'));
	return 0;
}
