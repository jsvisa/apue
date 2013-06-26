#ifndef RTSP_SERVER_H__
#define RTSP_SERVER_H__

#include "socketxx.h"

class RtspServer 
{
public:
	RtspServer();

	~RtspServer();

	bool Init();

	bool Start();
private:
	Socketxx servSock;
};

#endif
