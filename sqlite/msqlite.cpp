#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "msqlite.h"

MSqlite::MSqlite() {
	connected = false;
	hasStmt = false;
	hasRecord = false;
}

MSqlite::~MSqlite() {
	disconnect();
}
//���ӵ����ݿ�
bool MSqlite::connect(const char *path) {
	int rc = sqlite3_open(path, &db);
	if(rc != SQLITE_OK)
		return false;
	return connected = true;
}
//ִ��һ���ǲ�ѯ���
bool MSqlite::exec(const char *sql) {
	int rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
	
	//printf("sqlite3_exec=%d\n", rc);
	if(rc == SQLITE_OK) 
		return true;
	else
		return false;
}
//ִ��һ����ѯ���
bool MSqlite::query(const char *sql) {
	const char *tail;
	if(hasStmt) {
		freeResult();
		//hasStmt = false;
		//hasRecord = false;
	}
	int rc = sqlite3_prepare(db, sql, strlen(sql), &stmt, &tail);
//	printf("sqlite3_prepare=%d\n", rc);
	if(rc == SQLITE_OK) {
		hasStmt = true;
		return true;
	}
	else
		return false;

}
//��ȡһ����¼
bool MSqlite::fetchRecord() {
	int rc;
	if(!hasStmt)
		return false;
	rc = sqlite3_step(stmt);
	if(rc == SQLITE_ROW) {
		hasRecord = true;
	}
	else {
		hasRecord = false;
	}
	return hasRecord;
}
//��ȡĳһ�е�ֵ
bool MSqlite::getColumnData(int colNum, char *buf, int maxSize) {
	char *tmp;
	memset(buf, 0, maxSize);
	if(!hasRecord)
		return false;
	tmp = (char *)sqlite3_column_text(stmt, colNum);
//	snprintf(buf, maxSize, "%s", tmp);
	memcpy(buf, tmp, maxSize);
	return true;
}
//�ͷŲ�ѯ�Ľ����
bool MSqlite::freeResult() {
	if(hasStmt) {
		hasStmt = false;
		sqlite3_finalize(stmt);
	}
	hasRecord = false;
	return true;
}	
//�Ͽ����ݿ�����
bool MSqlite::disconnect() {
	if(connected) {
		freeResult();
		sqlite3_close(db);
		connected = false;
	}
	return true;
}
//�ж��Ƿ������������ݿ�
bool MSqlite::isConnected() {
	return connected;
}
//��ȡ���ݿ�����Ĵ�����Ϣ
const char *MSqlite::getErrString() {
	return sqlite3_errmsg(db);
}

//��ʼ����
void MSqlite::initLock() {
	pthread_mutex_init(&dblock, NULL);
}

//����
void MSqlite::lock() {
	pthread_mutex_lock(&dblock);
}
//����
void MSqlite::unlock() {
	pthread_mutex_unlock(&dblock);
}
//���Լ�����������
bool MSqlite::tryLock() {
	if(pthread_mutex_trylock(&dblock) == 0)
		return true;
	return false;
}
//�ͷ���
void MSqlite::destroyLock() {
	pthread_mutex_destroy(&dblock);
}
/*

*/

