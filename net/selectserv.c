#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "csocket.h"

#define MAX_CLIENTS 100
fd_set rfds; //要监听的读事件套接字
fd_set ofds; //保存原来的套接字fd_set的值
int maxNum = -1; //
int maxFD = -1;
int socks[MAX_CLIENTS];

int initSock(int port) {
	int i;
	int sock;
	for(i=0; i<MAX_CLIENTS; i++) {
		socks[i] = -1;
	}
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(myBind(sock, NULL, port) < 0) {
		perror("bind");
		return -1;
	}

	listen(sock, 100);
	socks[0] = sock;
	maxFD = sock;
	maxNum = 0;
	FD_ZERO(&ofds);
	FD_SET(socks[0], &ofds);
	return sock;
}
int mSend(int sock, const char *str, int len) {
	if(len == 0)
		len = strlen(str);
	return send(sock, str, len, 0);
}
int addSock(int fd) {
	int i;
	for(i=1; i<MAX_CLIENTS; i++) {
		if(socks[i] == -1) {
			socks[i] = fd;
			FD_SET(fd, &ofds);
			if(maxNum < i)
				maxNum = i;
			if(maxFD < fd)
				maxFD = fd;
			return 0;
		}
	}
	mSend(fd, "server is busy\r\n", 0);
	close(fd);
	return -1;
}
int removeSock(int i) {
	FD_CLR(socks[i], &ofds);
	close(socks[i]);
	socks[i] = -1;
}

void sendMsg(char *buf, int len) {
	int i;

	for(i=1; i<=maxNum; i++) {
		if(socks[i] >= 0) {
			send(socks[i], buf, len, 0);
		}
	}
}
int mSelect() {
	int ret;
	struct timeval tv;

	tv.tv_sec = 300;
	tv.tv_usec = 0;
	memcpy(&rfds, &ofds, sizeof(rfds));
	ret = select(maxFD+1, &rfds, NULL, NULL, &tv);
	return ret;
}
int main(int argc, char **argv) {
	int ret;
	int clientSock;
	int size;
	char buf[1024];
	char prefix[32];
	int i;
	int n;
	socklen_t len;
	struct sockaddr_in clientAddr;

	if(initSock(8000) < 0)
		return -1;

	while(1) {
		n = mSelect();
		if(FD_ISSET(socks[0], &rfds)) {
			size = sizeof(clientAddr);
			clientSock = accept(socks[0], (struct sockaddr *)&clientAddr, &size);
			if(clientSock < 0) {
				perror("Accept failed");
				continue;
			}
			addSock(clientSock);
		}
		for(i=1; i<=maxNum; i++) {
			if(socks[i] == -1)
				continue;
			if(FD_ISSET(socks[i], &rfds)) {
				size = recv(socks[i], buf, 1023, 0);
				if(size < 0) {
					if(errno == ECONNRESET)
						removeSock(i);
					else 
						perror("recv error");
				}
				else if(size == 0) {
					removeSock(i);
				}
				getpeername(socks[i], (struct sockaddr *)&clientAddr, &len);
				char *ip = inet_ntoa(clientAddr.sin_addr);
				sprintf(prefix, "[%s]:", ip);
				sendMsg(prefix, strlen(prefix));
				sendMsg(buf, size);
			}
		}
	}
	close(socks[0]);
	return 0;
}
