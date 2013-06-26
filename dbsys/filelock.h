#ifndef FILE_LOCK_H
#define FILE_LOCK_H

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

/* 设置一把读锁 */
extern int readLock(int fd, short start, short whence, short len);

/* 设置一把写锁 */
extern int writeLock(int fd, short start, short whence, short len);

/* 解锁 */
extern int unlock(int fd, short start, short whence, short len);

#endif
