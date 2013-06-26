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

	//execl("/bin/ls", "ls", "-l", 0);
	fd = open("./test.txt", O_CREAT|O_RDWR|O_APPEND, S_IRWXU);
	if(fd < 0) {
		perror("Open failed");
		return 1;
	}
	fd1 = dup2(fd, STDOUT_FILENO);
	fcntl(fd1, F_SETFD, 1);
	execl("/bin/ls", "ls", "-l", 0);
	return 0;
}
