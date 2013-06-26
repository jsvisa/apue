/* stdio3.c - ����dup */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

int main(int argc, char **argv) {
	int i;
	int fd, fd1;
	int size;
	char buf[1024*1024*10];
	struct timeval tv;

	fd = open("./test.f", O_CREAT|O_RDWR, S_IRWXU | S_IROTH);
	if(fd < 0) {
		printf("Create file failed:%s\n", strerror(errno));
		return 1;
	}
	//DUP�����ļ���д������
	write(fd, "123456", 6);
	fd1 = dup(fd);
	//��DUP�õ����ļ���������д������
	write(fd1, "abcdefg", 7);
	close(fd1);

	//�ر�ԭ�����ֺ���DUP�õ����ļ���������д������
	write(fd, "123456", 6);
	close(fd);

	return 0;
}
