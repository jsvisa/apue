#ifndef RTP_SESSION_H
#define RTP_SESSION_H

class RtpSession 
{
public:
	RtpSession();
	~RtpSession();
	bool GetRtpPort(int *ports);
	bool StartPlay();
	bool Pause();
	bool Terminate();
private:
	int rtpSock;
	int rtcpSock;
};

#endif
