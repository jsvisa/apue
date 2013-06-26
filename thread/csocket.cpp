#include <fcntl.h>
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
//		printf("Connect to %s:%d failed\n", host, port);
		close(sock);
		return -1;
	}
//	printf("Connect success\n");
	return sock;
}

int myBind(int sock, const char *ip, int port) {
	struct sockaddr_in  bindAddr;
	
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(port);
	if(ip)
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
				break;
			}
			break;
		default:
			return NULL;
	}
	return ip;
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
	socklen_t len;
	struct sockaddr sa;
	int res;
	int ret;
	
	tv.tv_sec=timeout;
	tv.tv_usec=0;
	FD_ZERO(&fds);
	FD_SET(sock, &fds);

	res = select(sock+1, &fds, NULL, NULL, &tv);
	if(res > 0 && FD_ISSET(sock, &fds)) {
		len = sizeof(sa);
		return accept(sock, &sa, &len);
	}
	
	return -1;
}
