#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	char buf[10];
	char buf1[20];

	char *buf2;
	memset(buf1, 0, sizeof(buf1));
	memset(buf, 0, 10);
	strcpy(buf, "abcde");
	buf[3] = 0;
	printf("buf = %s\n", buf);
	strcpy(buf1, buf);
//	memcpy(buf1, buf, 10);
	return 0;
}
