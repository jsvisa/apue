#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* ִ�У�./stdio6 0 < /dev/tty  ��׼����
 *       ./stdio6 1 > temp.txt  ��׼���
 *       cat temp.txt           �鿴��׼�����ģʽ
 *       ./stdio6 2 2 >>temp.txt ����׼���׷�ӵ�temp.txt
 *       ./stdio6 5 5<>temp.txt  5<>temp.txt��ʾ���ļ�������5�ϴ��ļ�temp.txt�Թ���д
 */
int main(int argc, char **argv) {
	int accessMode, val;
	int fd;

	if(argc !=2 ) {
		printf("Usage:%s <descriptor number>\n", "./stdio6");
		return 0;
	}
	
	fd = open(argv[1], O_CREAT|O_WRONLY|O_APPEND);
	if(fd < 0) {
		perror("open");
		return 0;
	}
	//if((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {
	if((val = fcntl(fd, F_GETFL, 0)) < 0) {
		printf("fcntl error for fd %d:%s\n", atoi(argv[1]), strerror(errno));
		return 1;
	}
	accessMode = val & O_ACCMODE;
	if(accessMode == O_RDONLY)
		printf("Read only\n");
	else if(accessMode == O_WRONLY)
		printf("Write only\n");
	else if(accessMode == O_RDWR)
		printf("Read and Write\n");
	else
		printf("Unknown access mode\n");

	if(val & O_APPEND) {
		printf("Append mode open\n");
	}
	if(val & O_NONBLOCK) {
		printf("Non block open\n");
	}
	return 0;
}

