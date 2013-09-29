#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "csocket.h"

int myConnect(const char *host,int port) {
	int sock;
	struct sockaddr_in  addr;
	memset(&addr, 0, sizeof(addr));
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	//convert a short from host byteorder to net byte order
	addr.sin_port = htons(port);
//	addr.sin_addr.s_addr =  inet_addr(host);
	inet_pton(AF_INET, host, &addr.sin_addr);

	if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
		printf("Connect to %s:%d failed\n", host, port);
		close(sock);
		return -1;
	}
	printf("Connect success\n");
	return sock;
}

int myBind(int sock, const char *ip, int port) {
	struct sockaddr_in  bindAddr;
	
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(port);
	if(ip != NULL)
		bindAddr.sin_addr.s_addr = inet_addr(ip);
	else
		bindAddr.sin_addr.s_addr = INADDR_ANY;

	unsigned long opt = 1;
	int len = sizeof(opt);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	return bind(sock, (struct sockaddr*)&bindAddr, sizeof(bindAddr));
}

int setBlock(int fd, int block) {
	int val;

	//获取文件描述字当前状态
	val = fcntl(fd, F_GETFL, 0);
	if(block == 1) //设置成阻塞模式
		val = val & (~O_NONBLOCK);
	else //设置成非阻塞模式
		val = val | O_NONBLOCK;
	//修改设置
	return fcntl(fd, F_SETFL, val);
}

char *getHostByName(const char *host, char *ip) {
	char **pptr; // string array
	struct hostent *hptr;

	memset(ip, 0, sizeof(ip));
	if((hptr = gethostbyname(host)) == NULL)
		return NULL;
	switch(hptr->h_addrtype) {
		case AF_INET:
#ifdef AF_INET6
		case AF_INET6:
#endif
			for(pptr=hptr->h_addr_list; *pptr!=NULL; pptr++) {
				//把一个网络字节顺序的整数IP转换成字符串的IP
				inet_ntop(hptr->h_addrtype, *pptr, ip, INET6_ADDRSTRLEN);
				//printf("ip=%s\n", ip);
				return ip;
			}
			break;
		default:
			return NULL;
	}
	return NULL;
}

int timeRecv(int fd, char* buf, int size, int timeout) {
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
	return recv(fd, buf, size, 0);
}

int timeConnect(const char *addr, int port, int timeout) {
	int fd;
	struct timeval tv;
	fd_set fds;
	struct sockaddr_in  saAddr;
	int res;
	int ret;

	memset(&saAddr, 0, sizeof(saAddr));
	
	fd = socket(AF_INET, SOCK_STREAM, 0);

	saAddr.sin_family = AF_INET;
	saAddr.sin_port = htons(port);
	inet_pton(AF_INET, addr, &saAddr.sin_addr);
//	saAddr.sin_addr.s_addr =  inet_addr(addr);

	fcntl(fd, F_SETFL, O_NONBLOCK);

	connect(fd, (struct sockaddr*)&saAddr, sizeof(saAddr));

	tv.tv_sec=timeout;
	tv.tv_usec=0;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	res = select(fd+1, NULL,  &fds, NULL, &tv);
	if(res > 0 && FD_ISSET(fd, &fds)) {
		int error;
		socklen_t len=sizeof(int); 
		getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len);

		if(error == 0) {
			fcntl(fd, F_SETFL, 0);
			return fd;
		}
	}
	
	fcntl(fd, F_SETFL, 0);
	close(fd);
	return -1;
}

int timeAccept(int sock, int timeout) {
	int fd;
	struct timeval tv;
	fd_set fds;
	int len;
	struct sockaddr_in sa;
	int res;
	int ret;
	
	tv.tv_sec=timeout;
	tv.tv_usec=0;
	FD_ZERO(&fds);
	FD_SET(sock, &fds);

	len = sizeof(sa);
	res = select(sock+1, &fds, NULL, NULL, &tv);
	if(res > 0 && FD_ISSET(sock, &fds)) {
		return accept(sock, (struct sockaddr *)&sa, &len);
	}
	
	return -1;
}

int fmtSend(int sock, int maxLen, const char *fmt, ...) {
	int len;
	va_list ap;
	char *line;

	line = (char *)malloc(maxLen+1);
	if(line == NULL) 
		return -1;
	va_start(ap, fmt);
	len = vsnprintf(line, maxLen, fmt, ap);
	va_end( ap );
	len = send(sock, line, len, 0);
	free(line);
	return len;
}

int Sendto(int sock, void *buf, int size, const char *ip, int port) {
	struct sockaddr_in serverAddr;
	short bytes;

	if(buf == NULL)
		return -1;

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &serverAddr.sin_addr);

	bytes = sendto(sock, buf, size, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	return bytes;	
}

int RecvFrom(int sock, void *buf, int size, char *ip, int *port) {
	int ret;
	int len;
	struct sockaddr_in addr;
	len = sizeof(addr);
	ret = recvfrom(sock, buf, size, 0, (struct sockaddr *)&addr, &len);
	if(ret <= 0) {
		return ret;
	}

	inet_ntop(AF_INET, &addr.sin_addr, ip, INET6_ADDRSTRLEN);
	*port = ntohs(addr.sin_port);
	return ret;
}

int TimeRecvFrom(int sock, void *buf, int size, struct sockaddr_in *addr, int ms) {
	int len;
	int ret;
	struct timeval tv;
	fd_set rfds;
	
	FD_ZERO(&rfds);
	FD_SET(sock, &rfds);
	tv.tv_sec = ms / 1000;
	tv.tv_usec = (ms % 1000)*1000;
	ret = select(sock+1, &rfds, NULL, NULL, &tv);

	if(ret > 0 && FD_ISSET(sock, &rfds)) {
		len = sizeof(addr);
		ret = recvfrom(sock, buf, size, 0, (struct sockaddr *)addr, &len);
		return ret;
	}

	return ret;
}

int SetRecvTimeOut(int sock, int sec, int usec) {
	int ret;
	struct timeval timeout;

	timeout.tv_sec = sec;
	timeout.tv_usec = usec;
	ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));	

	return ret;
}

