#ifndef MY_CSOCKET_H
#define MY_CSOCKET_H
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/un.h>

extern int myConnect(const char *address,int port);

extern int myBind(int sock, const char *ip, int port);

extern int setBlock(int sock, int block);

extern char *getHostByName(const char *host, char *ip);

extern int timeRecv(int fd, char* buf, int size, int timeout);

extern int timeConnect(const char *addr, int port, int timeout);

extern int timeAccept(int sock, int timeout);

#endif
