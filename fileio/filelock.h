#ifndef FILE_LOCK_H
#define FILE_LOCK_H

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

/* ����һ�Ѷ��� 
 * fd - �ļ�������
 * start - ���whenceλ�õ�ƫ����
 * whence - SEEK_SET, SEEK_CUR, SEEK_END
 * len - ���������ݳ��ȣ���λ�ֽ�
 * */
extern int readLock(int fd, short start, short whence, short len);

extern int readLocknw(int fd, short start, short whence, short len);
/* ����һ��д�� 
 * fd - �ļ�������
 * start - ���whenceλ�õ�ƫ����
 * whence - SEEK_SET, SEEK_CUR, SEEK_END
 * len - ���������ݳ��ȣ���λ�ֽ�
 * */
extern int writeLock(int fd, short start, short whence, short len);

extern int writeLocknw(int fd, short start, short whence, short len);

/* ���� */
extern int unlock(int fd, short start, short whence, short len);

#endif
