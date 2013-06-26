#include "filelock.h"

/* 设置一把读锁 */
int readLock(int fd, short start, short whence, short len) {
#if 0
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_start = start;
	lock.l_whence = whence;//SEEK_CUR,SEEK_SET,SEEK_END
	lock.l_len = len;
	lock.l_pid = getpid();

	if(fcntl(fd, F_SETLKW, &lock) == 0)
		return 1;
	
	return 0;
#endif
	return 1;
}

/* 设置一把写锁 */
int writeLock(int fd, short start, short whence, short len) {
#if 0
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_start = start;
	lock.l_whence = whence;
	lock.l_len = len;
	lock.l_pid = getpid();

	if(fcntl(fd, F_SETLKW, &lock) == 0)
		return 1;
	
	return 0;
#endif
	return 1;
}

/* 解锁 */
int unlock(int fd, short start, short whence, short len) {
#if 0
	struct flock lock;
	lock.l_type = F_UNLCK;
	lock.l_start = start;
	lock.l_whence = whence;
	lock.l_len = len;
	lock.l_pid = getpid();

	if(fcntl(fd, F_SETLK, &lock) == 0)
		return 1;

	return 0;
#endif
	return 1;
}
