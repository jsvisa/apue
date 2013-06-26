#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/un.h>
#include <stdio.h>

int connectUnix(const char* path) {
	int fd;
	struct sockaddr_un  addr;
	addr.sun_family = AF_UNIX;
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	strncpy(addr.sun_path, path, sizeof(addr.sun_path)-1);
	if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == 0)
		return fd;
	return -1;
}

int main(int argc, char **argv) {
	int fd;
	int size;
	char buf[1024];
	fd = connectUnix("/tmp/test.sock");
	if(fd < 0) {
		perror("connect");
		return 0;
	}
	size = recv(fd, buf, 1024, 0);
	if(size > 0) {
		buf[size] = 0;
		printf("%s\n", buf);
	}
}
