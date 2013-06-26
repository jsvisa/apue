#include <stdio.h>

int main(int argc, char **argv) {
	char buf[] = {"111112222233333"};
	memcpy(buf, buf+3, strlen(buf+3));
	printf("%s\n", buf);
	return 0;
}
