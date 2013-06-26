/* stdio.c - dup */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>



int main(int argc, char **argv) {
	int size;
	char buf[1024];
	int fd, fd1;

	/*fd = dup(STDOUT_FILENO);
	if(fd < 0) {
		printf("Dup failed:%s\n", strerror(errno));
		return 0;
	}
	*/

	fd = open("./log.txt", O_CREAT|O_RDWR|O_APPEND, S_IRWXU);
	if(fd < 0) {
		perror("Open failed");
		return 1;
	}
	dup2(fd, 1);
	close(fd);
	printf("2012-08-09 11:22:33 [debug]: dup2 ok\n");

//	close(STDOUT_FILENO);
//	fd1 = fcntl(fd, F_DUPFD, 1);
	return 0;
}
