#include "csocket.h"
#include <stdio.h>
#define MAXLINE 1024

void doClient(int sock) {
	int size;
	int maxfd;
	int ret;
	fd_set rset;
	char sendline[1024];
	char recvline[1024];
	struct timeval tv;
	FD_ZERO(&rset);

	maxfd = (sock>STDIN_FILENO?sock:STDIN_FILENO) + 1;
	for(;;) {
		tv.tv_sec = 20;
		tv.tv_usec = 0;
		write(STDOUT_FILENO, ">", 1);
		FD_SET(STDIN_FILENO, &rset);
		FD_SET(sock, &rset);
		ret = select(maxfd, &rset, NULL, NULL, &tv);
		if(ret <= 0) {
			printf("Recv timeout\n");
			break;
		}
		printf("tv.tv_sec=%d, tv.tv_usec=%d\n", tv.tv_sec, tv.tv_usec);
		if(FD_ISSET(sock, &rset)) { //socket is readable
			memset(recvline, 0, MAXLINE);
			if(recv(sock, recvline, MAXLINE, 0) <= 0) {
				printf("Recv return failed\n");
				return;
			}
			printf("[Client]:%s\n", recvline);
		}
		if(FD_ISSET(STDIN_FILENO, &rset)) {
			memset(sendline, 0, MAXLINE);
			size = read(STDIN_FILENO, sendline, MAXLINE);
			send(sock, "[Server]:", 9, 0);
			send(sock, sendline, strlen(sendline), 0); 
		}
	}
}

int main(int argc, char **argv) {
	int fd; 
	int len;
	char ip[32];
	char buf[1025];
	int port;
	int client;
	struct sockaddr sa;
	struct sockaddr_in sin;
	
	if(argc == 3) {
		strcpy(ip, argv[1]);
		port = atoi(argv[2]);
	}
	else {
		strcpy(ip, "127.0.0.1");
		port = 8000;
	}
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(myBind(fd, NULL, port) == 0) {
		printf("Bind to %s, %d success\n", ip, port);
	}
	else {
		printf("Bind to %s:%d failed\n", ip, port);
	}
	/////////////////////////////////////
	listen(fd, 1);
	while(1) {
		len = sizeof(sin);
		client = accept(fd, (struct sockaddr *)&sin, &len);
		inet_ntop(AF_INET, &(sin.sin_addr), buf, 64);
		printf("Accept connection: %s,%d\n", buf, ntohs(sin.sin_port));
		if(client > 0) {
			doClient(client);
			close(client);
		}
	}
	getchar();
	close(fd);

	return 0;
}

