#include "rtspserver.h"

int main(int argc, char **argv) 
{
	RtspServer server;
	if(!server.Init())
		return 1;

	server.Start();
	return 0;
}

