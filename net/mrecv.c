#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/un.h>

int myConnect(const char *address,int port) {
	int sock;
	struct sockaddr_in  addr;
	memset(&addr, 0, sizeof(addr));
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	//convert a short from host byteorder to net byte order
	addr.sin_port = htons(port);

	addr.sin_addr.s_addr =  inet_addr(address);
	if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
		printf("Connect to %s:%d failed\n", address, port);
		return -1;
	}
	printf("Connect success\n");
	return sock;
}

int main(int argc, char **argv) {
	int sock;
	int i;
	char buf[10240];
	int size;
	if(argc != 4) {
		printf("useage: connect ip port string\n");
		return 0;
	}
	sock = myConnect(argv[1], atoi(argv[2]));
	if(sock < 0) {
		printf("connect failed\n");
		return 1;
	}
	memset(buf, 0, sizeof(buf));
	size = send(sock, buf, sizeof(buf), 0);
	printf("send %d bytes\n", size);
	/*
	while(1) {
		recv(sock, &i, 4, 0); 
		printf("Recv:%d\n", i);
		usleep(500);
	}
	*/
	close(sock);

	return 0;
}
