#ifndef RTSP_SESSION_H
#define RTSP_SESSION_H

#include "socketxx.h"
#include "rtpsession.h"

class RtspSession {
public:
	RtspSession();
	~RtspSession();
	bool DoSession(int sockfd);
	bool GetRequest(char *req);
	bool DealRequest(char *req);
	bool DoOptions(char *req);
	bool DoDescribe(char *req);
	bool DoSetup(char *req);
	bool DoPlay(char *req);
	bool DoPause(char *req);
	bool DoTeardown(char *req);
private:
	Socketxx sock;
	RtpSession *rtpSession;
};

#endif
