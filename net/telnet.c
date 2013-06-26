#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/un.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>

int main(int argc, char **argv) {
	int size;
	int clientSock;
	int len;
	char buf[1024];
	char *p;
	int ret;
	int port = 23;
	char ipaddr[64];
	struct sockaddr_in servAddr;


	if(argc < 2) {
		printf("telnet [host] [port]\r\n");
		return 0;
	}
	if(argc == 3) {
		port = atoi(argv[2]);
	}

	clientSock = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servAddr, sizeof(servAddr));
	//getHostByName(argv[1],  ipaddr);

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(port);
	inet_pton(AF_INET, ipaddr, &(servAddr.sin_addr));
	len = sizeof(servAddr);
	printf("Connecting to %s, %d...\n", argv[1], port);
	if(connect(clientSock, (struct sockaddr *)&servAddr, len) == -1) {
		printf("connect to %s:%d failed\n", argv[1], port);
		close(clientSock);
		return 0;
	}
	printf("Connected.\n");

	while(1) {
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(STDIN_FILENO, &rfds);
		FD_SET(clientSock, &rfds);
		ret = select(clientSock+1, &rfds, NULL, NULL, NULL);
		if(ret <= 0) {
			perror("select");
			break;
		}
		if(FD_ISSET(STDIN_FILENO, &rfds)) {
			memset(buf, 0, 1024);
			size = read(STDIN_FILENO, buf, 1024);
			if(size > 0) {
				send(clientSock, buf, size, 0);
			}
			else {
				perror("read failed");
				break;
			}
		}
		if(FD_ISSET(clientSock, &rfds)) {
			memset(buf, 0, 1024);
			size = recv(clientSock, buf, 1024, 0);
			if(size > 0) {
				buf[size] = 0;
				write(STDOUT_FILENO, buf, size);
			}
			else {
				perror("recv failed");
				break;
			}
		}
	}
	close(clientSock);
	return 0;
}
