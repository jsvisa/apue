#include <stdio.h>
#include "csocket.h"
#include <errno.h>
#include <time.h>


int DoClient(int clientSock) {
	struct timeval tv;
	int size;
	char buf[1024];

	shutdown(clientSock, SHUT_RD);
	while(1) {
		size = recv(clientSock, buf, sizeof(buf)-1, 0);
		if(size <= 0) {
			printf("size = %d\n", size);
			printf("errno=%d\n", errno);
			if(errno == EAGAIN) {
				printf("It is recv timeout\n");
				continue;
			}
			else if(errno == EINTR) {
				printf("Received a signal\n");
				continue;
			}
			perror("recv");
			sleep(3);
		}
		
		send(clientSock, "hello\n", 6, 0);
		buf[size] = '\0';
		printf("Recv:%s", buf);
	}
	return 0;
}

int main(int argc, char **argv) {
	struct sockaddr_in clientAddr;
	int servSock;
	int clientSock;

	servSock = socket(AF_INET, SOCK_STREAM, 0);
	if(myBind(servSock, NULL, 8000) != 0) {
		perror("bind");
		return 1;
	}
	listen(servSock, 10);

	socklen_t len = sizeof(clientAddr);
	while(1) {
		clientSock = accept(servSock, (struct sockaddr *)&clientAddr, &len);
		if(clientSock > 0) {
			printf("Accepted\n");
			DoClient(clientSock);		
			close(clientSock);
		}
	}
}
