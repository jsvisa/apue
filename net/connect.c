#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/un.h>

int myConnect(const char *address,int port) {
	int sock;
	struct sockaddr_in  addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	inet_pton(AF_INET, address, &addr.sin_addr);
	//	addr.sin_addr.s_addr =  inet_addr(address);
	if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
		printf("Connect to %s:%d failed\n", address, port);
		return -1;
	}
	printf("Connect success\n");
	return sock;
}

int main(int argc, char **argv) {
	int size;
	int sock;
	char buf[80000];

	if (argc != 3)
	{
		printf("Argument error\n");
		return -1;
	}
	
	sock = myConnect(argv[1], atoi(argv[2]));

	if(sock < 0) {
		return 0;
	}

	size = recv(sock, buf, sizeof(buf), 0);
	if(size > 0) {
		buf[size] = 0; //essential
		printf("Recv %d bytes: %s\n", size, buf);
	}
	else {
		perror("Recv failed");
		close(sock); //essential
		return 0;
	}

	memset(buf, 'a', sizeof(buf));
	buf[7999] = '\0';
	size = send(sock, buf, sizeof(buf), 0);
	printf("send size=%d\n", size);

	close(sock);

	return 0;
}
