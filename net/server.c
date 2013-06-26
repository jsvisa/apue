#include "csocket.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 1024

void doClient(int sock) {
	int size;
	int maxfd;
	fd_set rset;
	char sendline[1024];
	char recvline[1024];
	FD_ZERO(&rset);

	send(sock, "Welcome!\n", strlen("Welcome!\n"), 0);
	for(;;) {
		FD_SET(sock, &rset);
		select(sock+1, &rset, NULL, NULL,NULL);
		if(FD_ISSET(sock, &rset)) { //socket is readable
			memset(recvline, 0, MAXLINE);
			if((size=recv(sock, recvline, MAXLINE, 0)) <= 0) {
				printf("Recv return failed\n");
				return;
			}
			printf("Client:%s\n", recvline);
			send(sock, recvline, size, 0);
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
	pid_t pid;
	struct sockaddr sa;
	struct sockaddr_in sin;
	
//	int iplong = inet_addr("192.168.0.6");
//	printf("iplong=%x\n", iplong);

	port = 8000;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(myBind(fd, NULL, port) == 0) {
		printf("Bind to %s, %d success\n", ip, port);
	}
	else {
		printf("Bind to %s:%d failed\n", ip, port);
	}
	signal(SIGCHLD, SIG_IGN);
	/////////////////////////////////////
	listen(fd, 10);
	while(1) {
		client = accept(fd, &sa, &len);
		if(client <= 0) {
			printf("Accept failed\n");
			break;
		}
		memcpy(&sin, &sa, len);
		printf("Accept connection: %s,%d\n", 
				inet_ntop(AF_INET, &(sin.sin_addr), buf, 64), ntohs(sin.sin_port));

		send(client, "Hello\r\n", 7, 0);
		pid = fork();
		if(pid == 0) { //parrent
			close(client);
			continue;
		}
		else if(pid > 0) {
			printf("I am child:%d\n", (int)getpid());
			if(client > 0) {
				close(fd);
				doClient(client);
				close(client);
			}
			exit(0);
		}
	}
	getchar();
	close(fd);
	return 0;
}

