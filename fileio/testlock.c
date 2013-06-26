#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
	int fd, start = 0, whence = 0, len=0;
	struct flock lock;
	char buf[1024];

	fd = open("a.txt", O_RDWR);
	if(fd < 0) {
		perror("Open failed");
		return 0;
	}
	lock.l_type = F_WRLCK;
	lock.l_start = start;
	lock.l_whence = whence;//SEEK_CUR,SEEK_SET,SEEK_END
	lock.l_len = len;
	lock.l_pid = getpid();
//  阻塞方式加锁
//	if(fcntl(fd, F_SETLKW, &lock) == 0)
//	//非阻塞方式加锁
	printf("Try to lock...\n");
	if(fcntl(fd, F_SETLKW, &lock) != 0) {
		perror("fcntl failed");
		return 0;
	}
	
//	memset(buf, 0, 1024);
//	read(fd, buf, 1024);
//	printf("Read:%s\n", buf);
	printf("Add lock success\n");
	//write(fd, "fdsafdsa", 10);
	getchar();
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	return 0;
}
