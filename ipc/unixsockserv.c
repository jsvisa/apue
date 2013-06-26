#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/un.h>
#include <stdio.h>


int main(int argc, char **argv) {
	int sock;
	int csock;
	char *path = "/tmp/test.sock";
	struct sockaddr_un  bindAddr;
	unlink(path);

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	bindAddr.sun_family = AF_UNIX;
	strncpy(bindAddr.sun_path, path, sizeof(bindAddr.sun_path)-1);

	unsigned long opt = 1;
	int len = sizeof(opt);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	if(bind(sock, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0) {
		perror("bind");
		return 0;
	}
	listen(sock, 10);

	csock = accept(sock, NULL, 0);
	printf("csock=%d\n", csock);
	send(csock, "hello\r\n", 7, 0);
	close(csock);
	return 0;
}
