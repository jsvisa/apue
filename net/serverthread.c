#include <pthread.h>
#include <signal.h>
#include "csocket.h"
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

void *sessionThread(void *arg) {
	int sock;
	sock = *(int *)arg;

	free(arg);
	doClient(sock);
	close(sock);
	return NULL;
}

int main(int argc, char **argv) {
	int fd; 
	int len;
	char ip[32];
	char buf[1025];
	int port;
	int client;
	pid_t pid;
	pthread_t tid;
	struct sockaddr sa;
	struct sockaddr_in sin;
	
//	int iplong = inet_addr("192.168.0.6");
//	printf("iplong=%x\n", iplong);

	if(argc == 3) {
		strcpy(ip, argv[1]);
		port = atoi(argv[2]);
	}
	else {
		strcpy(ip, "127.0.0.1");
		port = 8000;
	}
	signal(SIGPIPE, SIG_IGN);
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(myBind(fd, NULL, port) == 0) {
		printf("Bind to %s, %d success\n", ip, port);
	}
	else {
		printf("Bind to %s:%d failed\n", ip, port);
	}
	/////////////////////////////////////
	listen(fd, 1);
	int *sock;
	while(1) {
		client = accept(fd, &sa, &len);
		if(client <= 0) {
			printf("Accept failed\n");
			break;
		}
		sock = (int *)malloc(4);
		memcpy(sock, &client, 4);
		memcpy(&sin, &sa, len);
		printf("Accept connection: %s,%d\n", 
				inet_ntop(AF_INET, &(sin.sin_addr), buf, 64), ntohs(sin.sin_port));
		pthread_create(&tid, NULL, sessionThread, sock);
//		sleep(1);
	}
	getchar();
	close(fd);
	return 0;
}


