#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "csocket.h"

int DoClient(int clientSock) {
	struct timeval tv;
	int size;
	char buf[1024];

	tv.tv_sec = 10;
	tv.tv_usec = 0;
	if(setsockopt(clientSock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0) {
		perror("setsockopt");
		return 1;
	}
	
	while(1) {
		size = recv(clientSock, buf, sizeof(buf)-1, 0);
		if(size <= 0) {
			printf("size = %d\n", size);
			if(errno == EAGAIN) {
				printf("It is recv timeout\n");
				continue;
			}
			else if(errno == EINTR) {
				printf("Received a signal\n");
				continue;
			}
			perror("recv");
			break;
		}
		
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
	int size;
	char buf[101];
	while(1) {
		clientSock = accept(servSock, (struct sockaddr *)&clientAddr, &len);
		
		if(clientSock > 0) {
			printf("Accepted\n");
			SetRecvTimeOut(clientSock, 5, 0);
			size = recv(clientSock, buf, 100, 0);
			if(size <= 0) {
				if(errno == EAGAIN) {
					printf("EAGAIN\n");
				}
				else {
					printf("errno=%d, %s\n", errno, strerror(errno));
				}
			}

			else {
				buf[size] = 0;
				printf("Recv %s\n", buf);
			}
	//		DoClient(clientSock);		
			close(clientSock);
		}
	}
}

