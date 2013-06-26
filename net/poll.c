#include <limits.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <poll.h>
#include <sys/poll.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define MAXLINE 1024
#define SERV_PORT 8888

#define OPEN_MAX 100

typedef struct sockaddr SA;

int main(int argc, char **argv) {
	int i, maxi, listenfd, connfd, sockfd;
	int nready;
	ssize_t n;
	int k;

	char line[MAXLINE];
	socklen_t clilen;
	struct pollfd client[OPEN_MAX];
	struct sockaddr_in cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	/*
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	if(bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) != 0) {
		perror("Bind fail");
		close(listenfd);
		return 0;
	}
	*/
	if(myBind(listenfd, NULL, 8000) != 0) {
		perror("bind failed\n");
		close(listenfd);
		return 1;
	}
	listen(listenfd, 5);
	client[0].fd = listenfd;
	client[0].events = POLLIN;
	client[1].fd = STDIN_FILENO;
	client[1].events = POLLIN;
	
	for(i=2; i<OPEN_MAX; i++)	
		client[i].fd = -1;
	maxi = 1;

	for(;;) {
		nready = poll(client, maxi+1, 1800000);
		//当有新连接上来的时候，将新的连接加到数组中
		if(client[0].revents & POLLIN) { // Receive a new connection
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd, (SA *)&cliaddr, &clilen);
			for(i=2; i<OPEN_MAX; i++) {
				if(client[i].fd < 0) {
					client[i].fd = connfd;
					break;
				}
			}
			if(i == OPEN_MAX) {
				char *str = "Too many connections\n";
				send(connfd, str, strlen(str), 0);
				close(connfd);
				continue;
			}
			client[i].events = POLLIN;
			if(i > maxi)
				maxi = i;
			if(--nready <= 0)
				continue;
		}
		//检测标准输入是否有输入
		if(client[1].revents & POLLIN) {
			n = read(STDIN_FILENO, line, sizeof(line));
			for(i=2; i<=maxi; i++) {
				if((sockfd = client[i].fd) < 0)
					continue;
				send(sockfd, line, n, 0);
			}
			if(--nready <= 0)
				continue;
		}
		//检测是否有客户端发送了数据过来 
		for(i=2; i<=maxi; i++) {
			if((sockfd = client[i].fd) < 0)
				continue;
			if(client[i].revents & (POLLIN | POLLERR)) {
				if((n=recv(sockfd, line, MAXLINE, 0)) <= 0) {
					close(sockfd);
					client[i].fd = -1;
				}
				else if(n == 0) { //connection closed by client
					close(sockfd);
					client[i].fd = -1;
				}
				else {
					line[n] = '\0';
					printf("Recv from sock %d:%s", sockfd, line);
					for(k=1; k<=maxi; k++) {
						if(client[k].fd > 0) {
							send(client[k].fd, line, n, 0);
						}
					}
				}
				if(--nready <= 0)
					break;
			}
		}
	}
}
