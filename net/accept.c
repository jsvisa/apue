#include "csocket.h"
#include <stdio.h>
#include <sys/socket.h>
#define MAXLINE 1024

int main(int argc, char **argv) {
	int i;
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

	if(argc == 3) {
		strcpy(ip, argv[1]);
		port = atoi(argv[2]);
	}
	else {
		strcpy(ip, "127.0.0.1");
		port = 9000;
	}
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(myBind(fd, NULL, port) == 0) {
		printf("Bind to %s, %d success\n", ip, port);
	}
	else {
		printf("Bind to %s:%d failed\n", ip, port);
	}
	

	listen(fd, 1);

	while(1) {
		memset(&sin, 0, sizeof(sin));
		//client = accept(fd, &sa, &len);
		client = accept(fd, (struct sockaddr*)&sin, &len);
		if(client <= 0) {
			printf("Accept failed\n");
			break;
		}

		int buflen = 10;
		len = setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buflen, sizeof(buflen));
		if ( len < 0 )
		{
			perror("setsockopt");
			return -1;
		}
		printf("MSG_MAXIOVLEN IS %d\n", MSG_MAXIOVLEN);
		char *ptr = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\r\n";
		//memcpy(&sin, &sa, len);
		printf("Accept connection: %s,%d\n", 
				inet_ntop(AF_INET, &(sin.sin_addr), buf, 64), ntohs(sin.sin_port));

		while(1) {
			len = send(client, ptr, 100, 0);
			if (len < 0)
			{
				perror("send ");
				break;
			}
			sleep(1);
		}
		memset(buf, 0, 1024);
		recv(client, buf, 1024, 0);
		printf("Recv:%s\n", buf);
		close(client);
	}
	getchar();
	close(fd);

	return 0;
}

