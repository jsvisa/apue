#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>


struct udp_msg{
	int length;
	char msg[0];
};

int main( int argc, char **argv)
{
	int port;
	struct sockaddr_in server, client;
	struct udp_msg *pmsg;
	char   *buf;
	int    clientfd;
	int    ret;
	int    len, msg_len;
	char *host;

	if ( argc != 4){
		printf("udptest host port msg\n");
		return 0;
	}


	host = argv[1];
	port = atoi(argv[2]);
	buf = argv[3];
	server.sin_family = AF_INET;
	server.sin_port   = htons(port);
	inet_pton(AF_INET, host, &server.sin_addr);

	client.sin_family = AF_INET;
	client.sin_port  = htons(60002);
	client.sin_addr.s_addr = INADDR_ANY;

	clientfd = socket(PF_INET, SOCK_DGRAM, 0);
	if ( -1 == clientfd ){
		printf("create socket failed %s\n", strerror(errno));
		return -1;
	}

	ret = bind(clientfd, (struct sockaddr *)&client, sizeof(client));
	if ( -1 == ret){
		printf("bind failed %s \n", strerror(errno));
		return -1;
	}

	msg_len = strlen(buf);
	len = sizeof(server);
	ret = sendto(clientfd, buf, strlen(buf), 0,(struct sockaddr*) &server, len); 

	close(clientfd);
	return 0;
}
