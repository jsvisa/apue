#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	int size;
	int ret;
	int clientSock;
	int len;
	char buf[1024];
	char *p;
	struct sockaddr_in servAddr;
	clientSock = socket(AF_INET, SOCK_STREAM, 0);

	p = malloc(200*1024+1);
	bzero(&servAddr, sizeof(servAddr));
//	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(8888);
	inet_pton(AF_INET, "192.168.8.8", &(servAddr.sin_addr));
	len = sizeof(servAddr);
	if(connect(clientSock, (struct sockaddr *)&servAddr, len) == -1) {
		perror("connect failed");
		close(clientSock);
		free(p);
		return 0;
	}

	//send(clientSock, "Hello Server\n", 13, 0);
	while(1) {
		printf("Start recv...\n");
		size = recv(clientSock, p, 200*1024, 0);
		//size = recv(clientSock, buf, 1024, 0);
		if(size <= 0) {
			perror("recv failed");
			switch(errno) {
				case EAGAIN:
				printf("EAGAIN\n");
				case ECONNRESET:
				printf("ECONNRESET\n");
				case EINVAL:
				printf("EINVAL\n");
				case ENOTCONN:
				printf("ENOTCONN\n");
				case ENOTSOCK:
				printf("ENOTSOCK\n");
				case EOPNOTSUPP:
				printf("EOPNOTSUPP\n");
				case ETIMEDOUT:
				printf("ETIMEDOUT\n");
				case ENOBUFS:
				printf("ENOBUFS\n");
				case EIO:
				printf("EIO\n");
				case EINTR:
				printf("EINTR\n");
				case EBADF:
				printf("EBADF\n");
				case ESPIPE:
				printf("ESPIPE\n");
				default:
				printf("errno:%d\n", errno);
			}
			close(clientSock);
			free(p);
			return 0;
		}
	//	buf[size] = 0;
		p[size] = 0;
		printf("Recv %d bytes", size);
	}
	free(p);
	return 0;
}
