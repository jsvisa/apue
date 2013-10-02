#include <pthread.h>
#include "rtspserver.h"
#include "rtspsession.h"

void *ClientThread(void *arg) 
{
	int sock;

	pthread_detach(pthread_self());

	sock = (int)(long)arg;
	RtspSession session;
	session.DoSession(sock);
	return NULL;
}

RtspServer::RtspServer() 
{
}

RtspServer::~RtspServer() 
{
}

bool RtspServer::Init() 
{
	return true;
}

bool RtspServer::Start() 
{
	int sock;
	pthread_t tid;
	
	while(1)
	{
		sock = servSock.Accept(NULL);
		if(sock < 0) {
			//log("");
			continue;
		}
		pthread_create(&tid, NULL, ClientThread, (void *)sock);
	}

	return true;
}

