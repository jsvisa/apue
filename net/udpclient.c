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
	struct sockaddr_in server, client;
	struct udp_msg *pmsg;
	int    clientfd;
	int    ret;
	int i;
	//char *buf;
	int buf = 0x41424344;
	int    len, msg_len;
	int count=0;
	if (argc < 2)
	{
		printf("Need Argument.\n");
		return -1;
	}

//	buf = (char *)malloc(100*1024);
	clientfd = socket(PF_INET, SOCK_DGRAM, 0);
	if ( -1 == clientfd ){
		printf("create socket failed %s\n", strerror(errno));
		return -1;
	}
	client.sin_family = AF_INET;
	client.sin_port  = htons(60002);
	client.sin_addr.s_addr = INADDR_ANY;

	ret = bind(clientfd, (struct sockaddr *)&client, sizeof(client));
	if ( -1 == ret){
		printf("bind failed %s \n", strerror(errno));
		return -1;
	}

	server.sin_family = AF_INET;
	server.sin_port   = htons(60000);
	inet_pton(AF_INET, argv[1], &server.sin_addr);

	len = sizeof(client);
	for(i=0; i<5; i++) {
		//memset(buf, 'a'+i, 100*1024);
		//ret = sendto( clientfd, buf, 65550, 0,(struct sockaddr*) &server, len); 
		ret = sendto(clientfd, (void *)&buf, sizeof(buf), 0,
				     (struct sockaddr*) &server, len); 
		//ret = sendto(clientfd, buf, 500, 0,(struct sockaddr*) &server, len); 
		if(ret > 0) {
			count++;
			printf("sendto return:%d\n", ret);
		}
		else {
			perror("sendto");
		}
//		usleep(10);
	}

	printf("count=%d\n", count);
	close(clientfd);
	return 0;
}
