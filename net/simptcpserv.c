#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>

int mBind(int sock, char *ip, int port) {
	int len, ret;
	struct sockaddr_in servAddr, clientAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	//bzero(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	if(ip == NULL) {
		servAddr.sin_addr.s_addr = INADDR_ANY;
	}
	else {
		inet_pton(AF_INET, ip, &(servAddr.sin_addr));
	}
	/*
	unsigned long opt = 1;
	len = sizeof(opt);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	*/
	len = sizeof(servAddr);
	ret = bind(sock, (struct sockaddr *)&servAddr, len);
	if(ret < 0) {
		return ret;
	}
	return 0;
}

int main(int argc, char **argv) {
	int ret;
	int servSock;
	int clientSock;
	struct sockaddr_in clientAddr;
	int len;
	char buf[1024];
	char *p;

	servSock = socket(AF_INET, SOCK_STREAM, 0);
	if(mBind(servSock, NULL, atoi(argv[1])) != 0) {
		perror("Bind failed");
		close(servSock);
		return 0;
	}
	listen(servSock, 100);
	p = malloc(200*1024);
	memset(p, 'a', 200*1024);
	while(1) {
		clientSock = accept(servSock, (struct sockaddr *)&clientAddr, &len);
		printf("Client IP:%s\n", inet_ntoa(clientAddr.sin_addr));
		if(clientSock < 0) {
			perror("Accept failed");
			close(servSock);
			return 0;
		}
		while(1) {
			len = recv(clientSock, buf, 1024, 0);
			if(len <= 0) {
				close(clientSock);
				break;
			}
			buf[len] = 0;
			printf("Receive %d bytes:%s\n", len, buf);
			len = send(clientSock, p, 200*1024, 0);
			close(clientSock);
			break;
			/*
			send(clientSock, "You said:", 9, 0);
			printf("Send content to client\n");
			usleep(300);
			send(clientSock, buf, len, 0);
			*/
		}
	}
	close(servSock);
	free(p);
	return 0;
}
