#include <sys/time.h>
#include <sys/select.h>
#include "csocket.h"

#define MAXLINE 1024

int main(int argc, char **argv) {
	int size;
	int fd;
	int len;
	int sock;
	int client;
	char buf[1024];
	int maxfd;
	fd_set rset;
	char sendline[1024];
	char recvline[1024];
	int ret;
	struct sockaddr_in sin;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8000);
//	sin.sin_addr.s_addr = inet_addr("192.168.8.11");
	sin.sin_addr.s_addr = INADDR_ANY;
	if(bind(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("Bind");
		close(fd);
		return 1;
	}
	listen(fd, 1);
	len = sizeof(sin);
	while(1) {
		client = accept(fd, (struct sockaddr *)&sin, &len);
		printf("Accept connection from addr:%s, port %d\n",
			   	inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
		while(1) {
			struct timeval val;
			val.tv_sec = 30;
			val.tv_usec = 0;
			FD_ZERO(&rset);
			FD_SET(client, &rset);
			printf("wait for data...\n");
			ret = select(client+1, &rset, NULL, NULL, &val);
			if(ret == 0) {
				printf("Recv time out...\n");
				break;
			}
			else if(ret == -1) {
				perror("select");
				break;
			}
			
			printf("Data ready to recv\n");
			if(FD_ISSET(client, &rset)) {
				len = recv(client, buf, 1000, 0);
				buf[len] = 0;
				printf("Recv %s\n", buf);
				send(client, buf, len, 0);
			}
		}
		close(client);
	}
	return 0;
}
