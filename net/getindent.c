#include <stdio.h>
#include <netinet/in.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/* This program is used to test the byte order of the host */
struct MsgHeader 
{
	uint16_t cmd;
	int length;
	int mima;
};

int main(int argc, char **argv) 
{
	int i = 0x04030201;
	int n;
	char tmp[4];
	struct MsgHeader head;
	unsigned char *p;
	time_t t;

	memcpy(tmp, &i, 4);
	printf("Host Byte Order(%s endian):%d,%d,%d,%d\n", tmp[0]<tmp[1]?"little":"big", tmp[0], tmp[1], tmp[2], tmp[3]);
	n = htonl(i);
	memcpy(tmp, &n, 4);
	printf("Net Byte Order:%d,%d,%d,%d\n", tmp[0], tmp[1], tmp[2], tmp[3]);

	time(&t);
	head.cmd = 0xc2;
	head.length = 16;
	head.mima = t;
	p = malloc(sizeof(head));
	memcpy(p, &head, sizeof(head));

	printf("Head:");
	for(i=0; i<sizeof(head); i++) {
		printf("%02x ", p[i]);
	}
	printf("\n");
	free(p);

	return 0;
}
