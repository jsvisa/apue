#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int logined;

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

int GetRequest(int sock, char *buf) {
	int total = 0;
	int size;

	while(1) {
		if(recv(sock, buf+total, 1, 0) != 1) {
			return -1;
		}
		total++;
		if(buf[total-1] == '\n')
			break;
		if(total >= 4096) {
			return -1;
		}
	}
	buf[total] = '\0';
	printf("Request:%s", buf);
	return total;
}

int msend(int sock, char *buf) {
	return send(sock, buf, strlen(buf), 0);
}

int DoList(int sock, char *req) {
	printf("In %s\n", __func__);
	return 0;
}

int DoGet(int sock, char *req) {
	int fd;
	int size;
	char filename[256];
	char *p;
	struct stat st;
	char resp[256];

	printf("In %s\n", __func__);
	p = strchr(req, ' ');
	if(p == NULL) {
		msend(sock, "400 Invalid parameter\r\n");
		return 0;
	}
	memset(filename, 0, sizeof(filename));
	strncpy(filename, p+1, 255);

	p = strrchr(filename, '\n');	
	if(p != NULL) {
		*p = '\0';
	}
	p = strrchr(filename, '\r');	
	if(p != NULL) {
		*p = '\0';
	}

	fd = open(filename, O_RDONLY);
	if(fd < 0) {
		msend(sock, "500 Open file failed\r\n");
		return 0;
	}

	fstat(fd, &st);
	sprintf(resp, "200 OK %d\r\n", (int)st.st_size);
	msend(sock, resp);
	while(1) {
		size = read(fd, resp, sizeof(resp));
		if(size <= 0)
			break;
		send(sock, resp, size, 0);
	}
	close(fd);
	return 0;
}

int DoChdir(int sock, char *req) {
	printf("In %s\n", __func__);
	return 0;
}

int DoLogin(int sock, char *req) {
	printf("In %s\n", __func__);
	//TODO 验证用户名密码是否正确
	
	logined = 1;
	return 0;
}

int DoQuit(int sock, char *req) {
	printf("In %s\n", __func__);
	return 0;
}

int DealRequest(int sock, char *buf) {
	int ret = 0;
	printf("Deal Request\n");
	if(strncasecmp(buf, "login", 5) != 0 && logined != 1) {
		msend(sock, "401 Please login first\r\n");
		return -1;
	}

	if(strncasecmp(buf, "list", 4) == 0) {
		ret = DoList(sock, buf);
	}
	else if(strncasecmp(buf, "cd", 2) == 0) {
		ret = DoChdir(sock, buf);
	}
	else if(strncasecmp(buf, "login", 5) == 0) {
		ret = DoLogin(sock, buf);
	}
	else if(strncasecmp(buf, "get", 3) == 0) {
		ret = DoGet(sock, buf);
	}
	else if(strncasecmp(buf, "quit", 4) == 0) {
		ret = DoQuit(sock, buf);
	}
	else {
		printf("Unknown Request:%s\n", buf);
		msend(sock, "400 Unknown command\r\n");
	}
	return ret;

}

int DoClient(int sock) {
	char req[4096];
	int size;

	while(1) {
		size = GetRequest(sock, req);
		if(size < 0 ) {
			printf("Get request failed\n");
			return 0;
		}
		DealRequest(sock, req);
	}
}

int main(int argc, char **argv) {
	int sock;
	int clientSock;
	struct sockaddr_in addr, clientAddr;
	socklen_t len;
	char *tmp;
	time_t t;
	int size;
	char buf[1024];
	pid_t pid;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[1]));
	addr.sin_addr.s_addr = INADDR_ANY;

	len = sizeof(addr);
	if(bind(sock, (struct sockaddr *)&addr, len) < 0) {
		perror("Bind");
		return 1;
	}

	setBlock(STDIN_FILENO, 0);
	listen(sock, 10);
	while(1) {
		len = sizeof(clientAddr);
		clientSock = accept(sock, (struct sockaddr *)&clientAddr, &len);
		if(clientSock < 0) {
			perror("accept");
		}
		setBlock(clientSock, 0);
		while(1) {
			size = recv(clientSock, buf, sizeof(buf)-1, 0);
			if(size <= 0) {
				if(errno != EAGAIN) {
					printf("errno=%d\n", errno);
					printf("客户端连接已断开\n");
					close(clientSock);
					break;
				}
			}
			else {
				buf[size] = '\0';
				printf("%s", buf);
			}
			size = read(STDIN_FILENO, buf, sizeof(buf)-1);
			if(size > 0) {
				buf[size] = '\0';
				send(clientSock, buf, size, 0);
			}
			usleep(300);
		}
	}
	return 0;
}
