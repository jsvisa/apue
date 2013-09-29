#include <stdio.h>
#include <string.h>
#include "csocket.h"

#define MAXLINE 1024

void doClient(int sock) {
	int size;
	int maxfd;
	fd_set rset;
	char sendline[1024];
	char recvline[1024];
	FD_ZERO(&rset);

	maxfd = (sock>STDIN_FILENO?sock:STDIN_FILENO) + 1;
	for(;;) {
		write(STDOUT_FILENO, "#", 1);
		FD_SET(STDIN_FILENO, &rset);
		FD_SET(sock, &rset);
		select(maxfd, &rset, NULL, NULL,NULL);
		if(FD_ISSET(sock, &rset)) { //socket is readable
			memset(recvline, 0, MAXLINE);
			if(recv(sock, recvline, MAXLINE, 0) <= 0) {
				printf("Recv return failed\n");
				return;
			}
			printf("Server:%s\n", recvline);
//			write(STDOUT_FILENO, recvline, strlen(recvline));
		}
		if(FD_ISSET(STDIN_FILENO, &rset)) {
			memset(sendline, 0, MAXLINE);
			size = read(STDIN_FILENO, sendline, MAXLINE);
			printf("Readline %d:%s\n", size, sendline);
			send(sock, sendline, strlen(sendline), 0); 
		}
	}
}

int main(int argc, char **argv) {
	int client;
	int num;
	int size;
	int sock;
	char buf[1024];
	fd_set rfds;
	struct timeval tv;
	struct sockaddr addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(myBind(sock, NULL, 8000) != 0) {
		printf("Bind failed\n");
	}
	listen(sock, 5);
	
	while(1) {
		size = sizeof(addr);
		client = accept(sock, &addr, &size);
		if(client < 0) {
			perror("accept");
			continue;
		}
		while(1) {
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			FD_ZERO(&rfds);
			FD_SET(client, &rfds);
			num = select(client+1, &rfds, NULL, NULL, &tv);
			if(num <= 0) {
				printf("Not ready\n");
				continue;
			}
			if(FD_ISSET(client, &rfds)) {
				size = recv(client, buf, 100, 0);
				buf[size] = 0;
				printf("Recv %s\n", buf);
			}
		}
	}
	close(sock);

	return 0;

}
