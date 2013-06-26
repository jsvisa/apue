#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include "csocket.h"

int main( int argc, char **argv)
{
	int i;
	char ip[64];
	struct sockaddr_in server, client;
	char   buf[640];
	int    clientfd;
	int    ret;
	int    len, msg_len;
	int port;
	char *host;

	if ( argc != 3){
		printf("sendudp host port\n");
		return 0;
	}

	host = argv[1];
	port = atoi(argv[2]);
	printf("send to %s:%d\n", host, port);

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port   = htons(port);
	inet_pton(AF_INET, host, &server.sin_addr);

	clientfd = socket(PF_INET, SOCK_DGRAM, 0);
	if ( -1 == clientfd ){
		printf("create socket failed %s\n", strerror(errno));
		return -1;
	}

	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port  = htons(60002);
	client.sin_addr.s_addr = INADDR_ANY;
	ret = bind(clientfd, (struct sockaddr *)&client, sizeof(client));
	if ( -1 == ret){
		printf("bind failed %s \n", strerror(errno));
		return -1;
	}

	memset(buf, 'a', sizeof(buf));

	len = sizeof(server);
	/* 不停的发送数据包，观察丢包 */
	for(i=0; i<10000; i++) {
		ret = sendto(clientfd, buf, sizeof(buf), 0, (struct sockaddr*) &server, len); 
		if(ret < 0) {
			perror("sendto");
		}
		usleep(50);
	}

	/* 使用UDP向局域网内所有的IP发送数据包 */
	/*
	for(i=1; i<255; i++) {
		sprintf(ip, "192.168.0.%d", i);
		Sendto(clientfd, buf, sizeof(buf), ip, 60000);
		usleep(100);
	}
	*/

	/*
	while(1) {
		ret = recvfrom(clientfd, buf, 10, 0, (struct sockaddr*)&client, &len);
		if(ret > 0) {
			buf[ret] = 0;
			printf("Recv %d bytes %s\n", ret, buf);
		}
		else {
			perror("recvfrom");
		}
	}
	*/
	close(clientfd);
	return 0;
}
