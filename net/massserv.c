#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/un.h>

int main(int argc, char **argv) {
	int ret;
	int servSock;
	int clientSock;
	int len;
	char buf[1024];
	char *p;
	struct sockaddr_in servAddr, clientAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(8898);
//	servAddr.sin_addr.s_addr = inet_addr("192.168.8.8");
	inet_pton(AF_INET, "192.168.8.8", &(servAddr.sin_addr));
	servSock = socket(AF_INET, SOCK_STREAM, 0);
	len = sizeof(servAddr);
	ret = bind(servSock, (struct sockaddr *)&servAddr, len);
	if(ret < 0) {
		perror("Bind failed");
		close(servSock);
		return 0;
	}
	listen(servSock, 100);
	p = malloc(200*1024);
	memset(p, 'a', 200*1024);
	while(1) {
		clientSock = accept(servSock, (struct sockaddr *)&clientAddr, &len);
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
