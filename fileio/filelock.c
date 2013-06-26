#include "filelock.h"

/* ����һ�Ѷ��� */
int readLock(int fd, short start, short whence, short len) {
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_start = start;
	lock.l_whence = whence;//SEEK_CUR,SEEK_SET,SEEK_END
	lock.l_len = len;
	lock.l_pid = getpid();
//	//��������ʽ����
	if(fcntl(fd, F_SETLKW, &lock) == 0)
		return 1;
	
	return 0;
}

/* ����һ�Ѷ��� , ���ȴ� */
int readLocknw(int fd, short start, short whence, short len) {
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_start = start;
	lock.l_whence = whence;//SEEK_CUR,SEEK_SET,SEEK_END
	lock.l_len = len;
	lock.l_pid = getpid();
//  ��������ʽ����
	if(fcntl(fd, F_SETLK, &lock) == 0)
		return 1;
	
	return 0;
}
/* ����һ��д�� */
int writeLock(int fd, short start, short whence, short len) {
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_start = start;
	lock.l_whence = whence;
	lock.l_len = len;
	lock.l_pid = getpid();

	//������ʽ����
	//if(fcntl(fd, F_SETLKW, &lock) == 0)
	//��������ʽ����
	if(fcntl(fd, F_SETLKW, &lock) == 0)
		return 1;
	
	return 0;
}
/* ����һ��д�� */
int writeLocknw(int fd, short start, short whence, short len) {
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_start = start;
	lock.l_whence = whence;
	lock.l_len = len;
	lock.l_pid = getpid();

	//��������ʽ����
	if(fcntl(fd, F_SETLK, &lock) == 0)
		return 1;
	
	return 0;
}
/* ���� */
int unlock(int fd, short start, short whence, short len) {
	struct flock lock;
	lock.l_type = F_UNLCK;
	lock.l_start = start;
	lock.l_whence = whence;
	lock.l_len = len;
	lock.l_pid = getpid();

	if(fcntl(fd, F_SETLKW, &lock) == 0)
		return 1;

	return 0;
}
