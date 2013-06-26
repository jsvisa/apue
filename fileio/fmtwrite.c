#include <sys/types.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>

int fmtwrite(int fd, char *fmt, ...) {
	int len;
	va_list ap;
	char line[1024], newfmt[512];
	memset(newfmt, 0, sizeof(newfmt));
	memset(line, 0, sizeof(line));
	va_start(ap, fmt);
	printf("fmt=%s\n", fmt);
	len = vsnprintf(line, sizeof(line), fmt, ap);
	va_end(ap);

	write(fd, line, len);
}

int main(int argc, char **argv) {
	int fd, fd1;
	char buf[1024];
	int size;
	int pid;

	fd = open("./a.txt", O_CREAT|O_RDWR|O_APPEND, S_IRUSR|S_IRGRP);
	if(fd < 0) {
		perror("Open failed");
		return 1;
	}
	strcpy(buf, "log message");
	pid = getpid();
	fmtwrite(fd, "pid=%d,msg=%s, fd=%d\n", pid, buf, fd);
	printf("fd=%d\n", fd);

	char *fmt = "This test a=%s, b=%d, pid=%d\n";
	printf(fmt, buf, fd, pid);

	sleep(10);
	memset(buf, 0, sizeof(buf));
	size = read(fd, buf, 10);
	printf("read %d bytes:%s\n", size, buf);
	close(fd);
	return 0;
}
