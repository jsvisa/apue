#include "socketxx.h"
Socketxx::Socketxx() 
{
}

Socketxx::~Socketxx() 
{
}

bool Socketxx::Bind(int port) 
{
	return true;
}

bool Socketxx::Listen(int bklog) 
{
	return true;
}

int Socketxx::Accept(struct sockaddr *addr) 
{
	return -1;
}

bool Socketxx::Connect(const char *ip, int port, int timeout)
{
	return true;
}

int Socketxx::Read(void *buf, int size, int timeout) 
{
	return -1;
}

int Socketxx::ReadN(void *buf, int size, int timeout)
{
	return -1;
}

int Socketxx::Write(void *buf, int size, int timeout) 
{
	return -1;
}

