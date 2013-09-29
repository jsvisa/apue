#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "csocket.h"

#define MAXLINE 1024

void doClient(int sock) {
	int ret;
	int size;
	int maxfd;
	fd_set rset;
	char sendline[1024];
	char recvline[1024];
	FD_ZERO(&rset);

	for(;;) {
		write(STDOUT_FILENO, "#", 1);
		FD_SET(sock, &rset);
		ret = select(maxfd, &rset, NULL, NULL,NULL);
		if(ret == 0) {
		}
		else if(ret == -1) {
		}
		if(FD_ISSET(sock, &rset)) { //socket is readable
			memset(recvline, 0, MAXLINE);
			if((size = recv(sock, recvline, MAXLINE, 0)) <= 0) {
				printf("Recv return failed\n");
				return;
			}
			send(sock, "You:", 4, 0);
			send(sock, recvline, size, 0);
		}
	}
}

int main(int argc, char **argv) {
	int i;
	int fd; 
	int len;
	char ip[32];
	char buf[80000];
	int port;
	int client;
	int val = 1;
	pid_t pid;
	int size = 4;
	struct sockaddr_in clientAddr;
	struct sockaddr_in sin;
	
//	int iplong = inet_addr("192.168.0.6");
//	printf("iplong=%x\n", iplong);

	
	if(argc == 3) {
		strcpy(ip, argv[1]);
		port = atoi(argv[2]);
	}
	else {
		strcpy(ip, "127.0.0.1");
		port = 9000;
	}

	fd = socket(AF_INET, SOCK_STREAM, 0);

	/*
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(9000);
	sin.sin_addr.s_addr = INADDR_ANY; //绑定本地所有地址

//	inet_pton(AF_INET, "192.168.1.112", &sin.sin_addr);
//	sin.sin_addr.s_addr = inet_addr("192.168.8.11");

	if(bind(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("Bind");
		close(fd);
		return 1;
	}
    */

	if(myBind(fd, NULL, port) == 0) {
		printf("Bind to %s, %d success\n", ip, port);
	}
	else {
		printf("Bind to %s:%d failed\n", ip, port);
	}

	/////////////////////////////////////
	listen(fd, 10);
	len = sizeof(clientAddr);

	printf("Start accept\n");
	client = accept(fd, (struct sockaddr *)&clientAddr, &len);
	if(client < 0) {
		perror("Accept");
		close(fd);
		return 1;
	}

	printf("remote addr:%s, port %d\n", 
			inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

	for(i=0; i<10; i++) {
		send(client, "hello\r\n", 7, 0);
	}
/*
	while(1) {
		size = recv(client, buf, sizeof(buf), 0);
		if(size > 0) {
			buf[size] = 0;
			printf("Recv %d bytes: %s\n", size, buf);
		}

		sleep(1);
		printf("Closeed\n");
		close(client);

		
		pid = fork();
		if(pid > 0) {
			close(client);
			continue;
		}
		else if(pid == 0) {
			close(fd);
			doClient(client);
			exit(0);
		}
	}
*/	

	/*
	while(1) {
		client = accept(fd, &sa, &len);
		memcpy(&sin, &sa, len);
		printf("Accept connection: %s,%d\n", 
				inet_ntop(AF_INET, &(sin.sin_addr), buf, 64), ntohs(sin.sin_port));
		if(client > 0) {
			sprintf(buf, "HTTP/1.1 401 N/A\r\n"
					"Server: Router\r\n"
					"Connection: close\r\n"
					"WWW-Authenticate: Basic realm=\"TP-LINK Wireless Router WR340G\"\r\n"
					"Content-Type: text/html\r\n\r\n"
				   );

			send(client, buf, strlen(buf), 0);
//			doClient(client);
//
//			close(client);
		}
	}
	*/
	getchar();
	close(fd);

	return 0;
}

