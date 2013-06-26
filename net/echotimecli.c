#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <time.h>
int main(int argc, char **argv) {
	int sock;
	int clientSock;
	struct sockaddr_in addr, clientAddr;
	socklen_t len;
	char buf[101];
	int size;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(8765);
	inet_pton(AF_INET, "192.168.1.34", &addr.sin_addr);
	
	len = sizeof(addr);
	if(connect(sock, (struct sockaddr *)&addr, len) != 0) {
		perror("connect");
		return 1;
	}
	size = recv(sock, buf, 100, 0);
	if(size < 0) {
		perror("recv");
	}
	else {
		buf[size] = '\0';
		printf("Recv:%s\n", buf);
	}
	close(sock);
	return 0;
}
