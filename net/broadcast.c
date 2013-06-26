#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <pthread.h>

struct udp_msg{
	int length;
	char msg[0];
};

void *RecvThread(void *arg) {
	int sock;
	struct sockaddr_in addr;
	socklen_t len;
	int nSize;
	char buf[1024];
	sock = (int)arg;

	while(1) {
		len = sizeof(addr);

		nSize = recvfrom(sock, buf, 100, 0, (struct sockaddr *)&addr, &len);
		if(nSize > 0) {
			buf[nSize] = 0;
			printf("Find:%s\n", buf);
		}
	}
}

int main(int argc, char* argv[])
{
	int sockListener;
	struct sockaddr_in sin,saUdpServ;
	int fBroadcast = 1;
	char sendBuff[1024];
	int nSize;
	int ncount=0;
	struct sockaddr resAddr;
	struct timeval timeout;
	socklen_t len;
	pthread_t threadid;
	char *host = "255.255.255.255";
	//char *host = "255.255.255.255";
	sockListener=socket(AF_INET,SOCK_DGRAM,0);
	setsockopt(sockListener, SOL_SOCKET, SO_BROADCAST, &fBroadcast, sizeof(int));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8000);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind( sockListener, (struct sockaddr *)&sin, sizeof(sin))!=0)
	{
		printf("Can't bind socket to local port!Program stop.\n");//初始化失败返回-1
		return -1;
	}

	/*
	nSize = recvfrom(sockListener, sendBuff, 100, 0, &resAddr, &len);
	if(nSize > 0) {
		sendBuff[nSize] = 0;
		printf("RECV:%s\n", sendBuff);
	}
	*/

	saUdpServ.sin_family = AF_INET;
	inet_pton(AF_INET, host, &saUdpServ.sin_addr);
	saUdpServ.sin_port = htons (60000);//发送用的端口，可以根据需要更改
	nSize = sizeof ( struct sockaddr_in );

/*
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	nSize = sizeof(timeout);
	setsockopt(sockListener, SOL_SOCKET, SO_RCVTIMEO, &timeout, nSize);	
*/
//	pthread_create(&threadid, NULL, RecvThread, (void*)sockListener);
	while(1) {
		sprintf(sendBuff,"REQUEST IP\r\n");
		sendto(sockListener,sendBuff, strlen(sendBuff), 0, 
			(struct sockaddr*) &saUdpServ, sizeof (struct sockaddr_in));
		/*
		while(1) {
			len = sizeof(resAddr);
			nSize = recvfrom(sockListener, sendBuff, 100, 0, &resAddr, &len);
			if(nSize > 0) {
				sendBuff[nSize] = 0;
				printf("Find:%s\n", sendBuff);
			}
			else
				break;
		}
		*/
		sleep(3);
	}
	return 0;
}
