#include <stdio.h>
#include "csocket.h"
#define MAXLINE 1024

void doClient(int sock) {
	int size;
	int maxfd;
	fd_set rset;
	char sendline[1024];
	char recvline[1024];
	FD_ZERO(&rset);

	for(;;) {
		sleep(20);
		write(STDOUT_FILENO, "#", 1);
		FD_SET(STDIN_FILENO, &rset);
		FD_SET(sock, &rset);
		maxfd = (sock>STDIN_FILENO?sock:STDIN_FILENO) + 1;
		select(maxfd, &rset, NULL, NULL,NULL);
		if(FD_ISSET(sock, &rset)) { //socket is readable
			memset(recvline, 0, MAXLINE);
			if(recv(sock, recvline, MAXLINE, 0) <= 0) {
				printf("Recv return failed\n");
				return;
			}
			printf("Client:%s\n", recvline);
//			write(STDOUT_FILENO, "Client:", 7);
//			write(STDOUT_FILENO, recvline, strlen(recvline));
		}
		if(FD_ISSET(STDIN_FILENO, &rset)) {
			memset(sendline, 0, MAXLINE);
			size = read(STDIN_FILENO, sendline, MAXLINE);
			printf("Send to client:%s\n", sendline);
			send(sock, sendline, strlen(sendline), 0); 
		}
	}
}

int main(int argc, char **argv) {
	int fd; 
	int len;
	char buf[10240];
	int port;
	int client;
	int size;
	struct sockaddr sa;
	struct sockaddr_in sin;
	
	if(argc == 2) {
		port = atoi(argv[1]);
	}
	else {
		port = 8000;
	}
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(myBind(fd, NULL, port) == 0) {
		printf("Bind to %d success\n", port);
	}
	else {
		printf("Bind to %d failed\n", port);
	}
	/////////////////////////////////////
	listen(fd, 1);
	char i = 0;
	client = timeAccept(fd, 100);
	if(client < 0) {
		printf("Accept failed\n");
		return 0;
	}

	sleep(1);
	while(1) {
		size = recv(client, buf, sizeof(buf), 0);
		printf("Recv:%d bytes\n", size);
		if(size <=0)
			break;
	}
	/*
	for(i=31; i<60; i++) {
		send(client, &i, 1, 0);
		sleep(1);
	}
	*/
	close(fd);

	return 0;
}

