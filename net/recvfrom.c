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
	struct sockaddr_in client;
	char   buf[100000];
	int    clientfd;
	int    ret;
	int    len, msg_len;
	int port;
	char *host;

	clientfd = socket(PF_INET, SOCK_DGRAM, 0);
	if ( -1 == clientfd ){
		printf("create socket failed %s\n", strerror(errno));
		return -1;
	}

	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port  = htons(60000);
	client.sin_addr.s_addr = INADDR_ANY;
	ret = bind(clientfd, (struct sockaddr *)&client, sizeof(client));
	if ( -1 == ret){
		printf("bind failed %s \n", strerror(errno));
		return -1;
	}

	i = 0;
	while(1) {
		ret = recvfrom(clientfd, buf, sizeof(buf), 0, (struct sockaddr*)&client, &len);
		if(ret > 0) {
			buf[ret] = 0;
			printf("Recv %d bytes, %d packets\n", ret, ++i);
		}
	}
	close(clientfd);
	return 0;
}
