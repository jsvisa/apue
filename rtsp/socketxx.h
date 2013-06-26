#ifndef SOCKETXX_H
#define SOCKETXX_H

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

class Socketxx 
{
public:
	Socketxx();
	~Socketxx();
	bool Bind(int port);
	bool Listen(int bklog);
	int Accept(struct sockaddr *addr);
	bool Connect(const char *ip, int port, int timeout=0);
	int Read(void *buf, int size, int timeout);
	int ReadN(void *buf, int size, int timeout);
	int Write(void *buf, int size, int timeout);
private:
	int sock;
};

#endif

