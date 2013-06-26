#include <arpa/inet.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if(argc !=2) {
		printf("inetaddr <ipaddr>\n");
		return 0;
	}
	printf("inet_addr(%s)=%x\n", argv[1], inet_addr(argv[1]));
	struct in_addr addr;
	inet_aton(argv[1], &addr);
	printf("inet_aton(%s)=%x\n", argv[1], addr.s_addr);

	return 0;
}
