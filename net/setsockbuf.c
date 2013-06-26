#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>

int main( int argc, char **argv)
{
	int i;
	struct sockaddr_in server, client;
	char   buf[10000];
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

	int buflen = 2024*1024;
	int getBufLen;
	int size = 4;
	len = getsockopt(clientfd, SOL_SOCKET, SO_SNDBUF, &getBufLen, &size);
	if (len < 0)
	{
		perror("getsockopt");
	}
	else
	{
		printf("sendbuflen is %d\n", getBufLen);
	}

	len = getsockopt(clientfd, SOL_SOCKET, SO_RCVBUF, &getBufLen, &size);
	if (len < 0)
	{
		perror("getsockopt");
	}
	else
	{
		printf("sendbuflen recf buf len is %d\n", getBufLen);
	}
	//setsockopt(sock, SOL_SOCKET,SO_RCVBUF, &n, sizeof(n));
	len = setsockopt(clientfd, SOL_SOCKET, SO_SNDBUF, &buflen, sizeof(buflen));
	if(len < 0) {
		perror("setsockopt");
	}

	len = getsockopt(clientfd, SOL_SOCKET, SO_SNDBUF, &getBufLen, &size);
	if (len < 0)
	{
		perror("getsockopt");
	}
	else
	{
		printf("after setsockopt sendbuflen is %d\n", getBufLen);
	}

	len = sizeof(server);
	ret = sendto(clientfd, buf, sizeof(buf), 0, (struct sockaddr*) &server, len); 
	if(ret < 0) {
		perror("sendto");
	}

	close(clientfd);
	return 0;
}

