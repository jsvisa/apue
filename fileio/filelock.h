#ifndef FILE_LOCK_H
#define FILE_LOCK_H

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

/* 设置一把读锁 
 * fd - 文件描述符
 * start - 相对whence位置的偏移量
 * whence - SEEK_SET, SEEK_CUR, SEEK_END
 * len - 加锁的数据长度，单位字节
 * */
extern int readLock(int fd, short start, short whence, short len);

extern int readLocknw(int fd, short start, short whence, short len);
/* 设置一把写锁 
 * fd - 文件描述符
 * start - 相对whence位置的偏移量
 * whence - SEEK_SET, SEEK_CUR, SEEK_END
 * len - 加锁的数据长度，单位字节
 * */
extern int writeLock(int fd, short start, short whence, short len);

extern int writeLocknw(int fd, short start, short whence, short len);

/* 解锁 */
extern int unlock(int fd, short start, short whence, short len);

#endif
