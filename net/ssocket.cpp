#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include "ssocket.hh"

#ifdef USE_SOCKLEN_T
typedef socklen_t SOCKLEN_T;
#else
typedef int SOCKLEN_T;
#endif

SSocket::SSocket(int ifd) {
	fd = ifd;
}

SSocket::~SSocket() {
}

unsigned long SSocket::getIPLong() const {
	struct sockaddr_in  sockAddr;
	SOCKLEN_T nSockAddrLen = sizeof(sockAddr);
	memset(&sockAddr, 0, sizeof(sockAddr));

	if (getpeername(fd, (struct sockaddr*)&sockAddr, (socklen_t *)&nSockAddrLen)<0)
			return 0;
	return sockAddr.sin_addr.s_addr;
}


bool SSocket::getAddrBySocket(char *address) {
	return getAddrBySocket(fd, address);
}

bool SSocket::getAddrBySocket(int ifd, char *address) {
	struct sockaddr_in  sockAddr;
	SOCKLEN_T nSockAddrLen = sizeof(sockAddr);

	if(address == NULL)
		return false;
	memset(&sockAddr, 0, sizeof(sockAddr));


	if (getpeername(ifd, (struct sockaddr*)&sockAddr, (socklen_t *)&nSockAddrLen)<0)
		return false;
		
	char buf[16];
	memset(buf, 0, 16);
	strncpy(buf, inet_ntoa(sockAddr.sin_addr), 15);
	strcpy(address, buf);
	return true;
}

int SSocket::sendto(
		const char * ip, 
		int port, 
		const char *buf, 
		int size) {

	struct sockaddr_in serverAddr;
	short bytes;

	if(buf == NULL)
		return -1;

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(ip);

	if(size <= 0)
		size = strlen(buf);
	
	bytes = ::sendto(fd,
				buf,
				size,
				0,
				(struct sockaddr*)&serverAddr,
				sizeof(serverAddr));

	return bytes;	
}

int SSocket::sendto(
		unsigned long ip,
		int port, 
		const char *buf, 
		int size) {

	struct sockaddr_in serverAddr;
	short bytes;

	if(buf == NULL)
		return -1;

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(ip);

	if(size <= 0)
		size = strlen(buf);
	
	bytes = ::sendto(fd,
				buf,
				size,
				0,
				(struct sockaddr*)&serverAddr,
				sizeof(serverAddr));

	return bytes;	
}


int SSocket::recvfrom(
		unsigned long &ip,
		int &port,
		char* buf, 
		int size, 
		int timeout) {

	struct timeval waitTime;
	struct sockaddr_in fromAddr;
	short bytes = 0;
	fd_set readFDs;
	int res;

	SOCKLEN_T fromLen = sizeof(fromAddr);

	if(timeout > 0) {
		waitTime.tv_sec = timeout;
		waitTime.tv_usec = 0;
		memset(&fromAddr, 0, fromLen);
		FD_ZERO(&readFDs);
		FD_SET(fd, &readFDs);
		res = select(fd+1, &readFDs, NULL, NULL, &waitTime);
		if(res <= 0 || !FD_ISSET(fd, &readFDs))
			return -1;
	}

	bytes = ::recvfrom(fd, 
				buf, 
				size, 
				0, 
				(struct sockaddr*)&fromAddr, 
				&fromLen);
	port = ntohs(fromAddr.sin_port);
	ip = ntohl(fromAddr.sin_addr.s_addr);      
	return bytes;
}


int SSocket::recvfrom(
		char  *ip,
		int nsize,
		int &port,
		char* buf, 
		int size, 
		int timeout) {

	struct timeval waitTime;
	struct sockaddr_in fromAddr;
	short bytes = 0;
	fd_set readFDs;
	SOCKLEN_T fromLen = sizeof(fromAddr);
	int res = -1;

	if(timeout > 0) {
		waitTime.tv_sec = timeout;
		waitTime.tv_usec = 0;
		memset(&fromAddr, 0, fromLen);
		FD_ZERO(&readFDs);
		FD_SET(fd, &readFDs);
		res = select(fd+1, &readFDs, NULL, NULL, &waitTime);
		if(res <= 0 || !FD_ISSET(fd, &readFDs))
			return -1;
	}

	bytes = ::recvfrom(fd, 
				buf, 
				size, 
				0, 
				(struct sockaddr*)&fromAddr, 
				&fromLen);
	port = ntohs(fromAddr.sin_port);
	strncpy(ip, inet_ntoa(fromAddr.sin_addr), nsize);
	return bytes;
}

int SSocket::recvfrom(
		char *ip,
		int nsize,
		int &port,
		char* buf, 
		int size) {

	struct sockaddr_in fromAddr;
	short bytes = 0;
	SOCKLEN_T fromLen = sizeof(fromAddr);

	memset(&fromAddr, 0, fromLen);

	bytes = ::recvfrom(fd,
				buf,
				size,
				0,
				(struct sockaddr*)&fromAddr,
				&fromLen);
	if(bytes >= 0) {
		port=ntohs(fromAddr.sin_port);
		strncpy(ip, inet_ntoa(fromAddr.sin_addr), nsize);
	}
	return bytes;
}


int SSocket::recvfrom(
		unsigned long &ip,
		int &port,
		char* buf, 
		int size) {

	struct sockaddr_in fromAddr;
	short bytes = 0;
	SOCKLEN_T fromLen = sizeof(fromAddr);
	memset(&fromAddr,0,fromLen);

	bytes = ::recvfrom(fd,
				buf,
				size,
				0,
				(struct sockaddr*)&fromAddr,
				&fromLen);
	if(bytes >= 0) {
		port = ntohs(fromAddr.sin_port);
		ip = ntohl(fromAddr.sin_addr.s_addr);      
	}
	return bytes;
}


int SSocket::recvfrom(
		char* buf, 
		int size, 
		int timeout) {

	struct timeval waitTime;
	struct sockaddr_in fromAddr;
	fd_set readFDs;
	int res;
	SOCKLEN_T fromLen = sizeof(fromAddr);

	if(timeout > 0) {
		waitTime.tv_sec = timeout;
		waitTime.tv_usec = 0;
		memset(&fromAddr, 0, fromLen);
		FD_ZERO(&readFDs);
		FD_SET(fd, &readFDs);
		res = select(fd+1, &readFDs, NULL, NULL, &waitTime);
		if(res <= 0 && !FD_ISSET(fd, &readFDs))
			return -1;
	}

	return ::recvfrom(fd, 
				buf, 
				size, 
				0, 
				(struct sockaddr*)&fromAddr, 
				&fromLen);
}


int SSocket::recvfrom(
		char* buf, 
		int size) {

	struct sockaddr_in fromAddr;
	SOCKLEN_T fromLen = sizeof(fromAddr);
	memset(&fromAddr, 0, fromLen);
	return ::recvfrom(fd, 
				buf, 
				size, 
				0, 
				(struct sockaddr*)&fromAddr, 
				&fromLen);
}

	
int SSocket::recv(
		char* buf, 
		int size, 
		int timeout) {
	
	struct timeval waitTime;
	fd_set readFDs;
	int res;

	if(timeout > 0) {
		waitTime.tv_sec = timeout;
		waitTime.tv_usec = 0;
		FD_ZERO(&readFDs);
		FD_SET(fd, &readFDs);
		res = select(fd+1, &readFDs, NULL, NULL, &waitTime);
		if(res <= 0 && !FD_ISSET(fd, &readFDs))
			return -1;
	}
	return ::recv(fd, buf, size, 0);
}

int SSocket::recv(
		char* buf, 
		int size) {
	return ::recv(fd, buf, size, 0);
}

int SSocket::acceptFD( ) {
	int ifd;
	ifd = ::accept(fd,NULL,NULL);
	return ifd;
}

SSocket SSocket::accept( ) {
	int ifd;
	ifd = ::accept(fd,NULL,NULL);
	return SSocket(ifd);
}

SSocket SSocket::accept(int timeout) {
	int ifd = -1;
	struct timeval waitTime;
	fd_set readFDs;
	int res;

	if(timeout > 0) {
		waitTime.tv_sec=timeout;
		waitTime.tv_usec=0;
		
		FD_ZERO(&readFDs);
		FD_SET(fd, &readFDs);
		res = select(fd+1, &readFDs, NULL, NULL, &waitTime);
		if(res <= 0 && !FD_ISSET(fd, &readFDs)) 
			ifd = -1;
	}

	ifd = ::accept(fd, NULL, NULL);
	return SSocket(ifd);
}

int SSocket::acceptFD(int timeout) {
	struct timeval waitTime;
	fd_set readFDs;
	int res;

	if(timeout > 0) {
		waitTime.tv_sec=timeout;
		waitTime.tv_usec=0;
		
		FD_ZERO(&readFDs);
		FD_SET(fd, &readFDs);
		res = select(fd+1, &readFDs, NULL, NULL, &waitTime);
		if(res <= 0 && !FD_ISSET(fd, &readFDs)) 
			return -1;
	}

	return ::accept(fd, NULL, NULL);
}

bool SSocket::shutdown(int mode) {
	if(::shutdown(fd, mode) != 0)
		return false;
	return true;
}

int SSocket::create(int type) {
	switch(type) {
	case SOCKET_TCP:
		return fd = socket(AF_INET, SOCK_STREAM, 0);
	case SOCKET_UDP:
		return fd = socket(AF_INET, SOCK_DGRAM, 0);
	case SOCKET_UNIX_STREAM:
		return fd = socket(PF_UNIX, SOCK_STREAM, 0);
	case SOCKET_UNIX_DGRAM:
		return fd = socket(PF_UNIX, SOCK_DGRAM, 0);
	}
	return -1;
}

bool SSocket::connect(const char *address,int port) {
	/*
	char address[32];
	if(!getAddrByName(host, address))
		return false;
	*/

	struct sockaddr_in  addr;
	memset(&addr, 0, sizeof(addr));
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr =  inet_addr(address);
	if(::connect(fd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
		return false;
	return true;
}

bool SSocket::connect(const char *addr, int port, int timeout) {
	/*
	char addr[32];
	if(!getAddrByName(host, addr))
		return false;
	*/

	struct timeval tv;
	fd_set fds;
	struct sockaddr_in  saAddr;
	int res;
	bool ret = false;

	memset(&saAddr, 0, sizeof(saAddr));
	
	saAddr.sin_family = AF_INET;
	saAddr.sin_port = htons(port);
	saAddr.sin_addr.s_addr =  inet_addr(addr);

	fcntl(fd, F_SETFL, O_NONBLOCK);

	::connect(fd, (struct sockaddr*)&saAddr, sizeof(saAddr));

	tv.tv_sec=timeout;
	tv.tv_usec=0;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	res = select(fd+1, NULL,  &fds, NULL, &tv);
	if(res > 0 && FD_ISSET(fd, &fds)) {
		int error;
		SOCKLEN_T len=sizeof(int); 
		getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len);

		if(error == 0)
			ret = true; 
		else
			ret = false; 

		fcntl(fd, F_SETFL, 0);
		return ret;
	}
	fcntl(fd, F_SETFL, 0);
	return false;
}


int SSocket::send(const char *buf,int size) {
	if(buf == NULL)
		return -1;
	if(size <= 0)
		size = strlen(buf);
	return ::send(fd, buf, size, 0);
}

bool SSocket::close( ) {
	if(fd >= 0)
		::close(fd);
	fd = -1;
	return true;
}	

bool SSocket::sClose() {
	if(fd >= 0)
		::close(fd);
	fd = -1;
	return true;
}

bool SSocket::listen(int n) {
	if(::listen(fd, n) != 0) 
		return false;
	return true;
}

bool SSocket::bind(const char* path) {
	
	struct sockaddr_un  bindAddr;
	unlink(path);
	bindAddr.sun_family = AF_UNIX; //AF_INET, AF_INET6
	strncpy(bindAddr.sun_path, path, sizeof(bindAddr.sun_path)-1);

	unsigned long opt = 1;
	int len = sizeof(opt);
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	if(::bind(fd, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0)
		return false;
	return true;
}
	
bool SSocket::connectUnix(const char* path) {
	struct sockaddr_un  addr;
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path)-1);
	if(::connect(fd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
		return false;
	return true;
}
	
bool SSocket::connectUnix(const char *path, int timeout) {
	struct timeval tv;
	fd_set fds;
	struct sockaddr_un  saAddr;
	int res;
	bool ret = false;

	memset(&saAddr, 0, sizeof(saAddr));
	
	saAddr.sun_family = AF_UNIX;
	strncpy(saAddr.sun_path, path, sizeof(saAddr.sun_path)-1);

	fcntl(fd, F_SETFL, O_NONBLOCK);

	::connect(fd, (struct sockaddr*)&saAddr, sizeof(saAddr));

	tv.tv_sec=timeout;
	tv.tv_usec=0;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	res = select(fd+1, NULL,  &fds, NULL, &tv);
	if(res > 0 && FD_ISSET(fd, &fds)) {
		int error;
		SOCKLEN_T len=sizeof(int); 
		getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len);

		if(error == 0)
			ret = true; 
		else
			ret = false; 

		fcntl(fd, F_SETFL, 0);
		return ret;
	}
	fcntl(fd, F_SETFL, 0);
	return false;
}


bool SSocket::bind(int port) {
	struct sockaddr_in  bindAddr;
	
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(port);
	bindAddr.sin_addr.s_addr = INADDR_ANY;

	unsigned long opt = 1;
	int len = sizeof(opt);
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	if(::bind(fd, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0)
		return false;
	return true;
}


bool SSocket::bind(const char *ip, int port) {
	struct sockaddr_in  bindAddr;
	
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(port);
	bindAddr.sin_addr.s_addr = inet_addr(ip);

	unsigned long opt = 1;
	int len = sizeof(opt);
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	if(::bind(fd, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0)
		return false;
	return true;
}

bool SSocket::available() {
	if(fd < 0)
		return false;
	return true;
}

bool SSocket::setBlock(int block) {
	int val;

	val = fcntl(fd, F_GETFL, 0);
	if(block == 1)
		val = val & (~O_NONBLOCK);
	else
		val = val | O_NONBLOCK;
	if(fcntl(fd, F_SETFL, val) == 0)
		return true;
	return false;
}

bool SSocket::getHostByName(const char *host, char *ip) {
	char **pptr;
	struct hostent *hptr;

	if((hptr = gethostbyname(host)) == NULL)
		return false;
	switch(hptr->h_addrtype) {
		case AF_INET:
#ifdef AF_INET6
		case AF_INET6:
#endif
			for(pptr=hptr->h_addr_list; *pptr!=NULL; pptr++) {
				inet_ntop(hptr->h_addrtype, *pptr, ip, INET6_ADDRSTRLEN);
				break;
			}
			break;
		default:
			return false;
	}
	return true;
}
