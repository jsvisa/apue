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
//连接到数据库
bool MSqlite::connect(const char *path) {
	int rc = sqlite3_open(path, &db);
	if(rc != SQLITE_OK)
		return false;
	return connected = true;
}
//执行一条非查询语句
bool MSqlite::exec(const char *sql) {
	int rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
	
	//printf("sqlite3_exec=%d\n", rc);
	if(rc == SQLITE_OK) 
		return true;
	else
		return false;
}
//执行一条查询语句
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
//获取一条记录
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
//获取某一列的值
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
//释放查询的结果集
bool MSqlite::freeResult() {
	if(hasStmt) {
		hasStmt = false;
		sqlite3_finalize(stmt);
	}
	hasRecord = false;
	return true;
}	
//断开数据库连接
bool MSqlite::disconnect() {
	if(connected) {
		freeResult();
		sqlite3_close(db);
		connected = false;
	}
	return true;
}
//判断是否已连接上数据库
bool MSqlite::isConnected() {
	return connected;
}
//获取数据库操作的错误信息
const char *MSqlite::getErrString() {
	return sqlite3_errmsg(db);
}

//初始化锁
void MSqlite::initLock() {
	pthread_mutex_init(&dblock, NULL);
}

//加锁
void MSqlite::lock() {
	pthread_mutex_lock(&dblock);
}
//解锁
void MSqlite::unlock() {
	pthread_mutex_unlock(&dblock);
}
//尝试加锁，非阻塞
bool MSqlite::tryLock() {
	if(pthread_mutex_trylock(&dblock) == 0)
		return true;
	return false;
}
//释放锁
void MSqlite::destroyLock() {
	pthread_mutex_destroy(&dblock);
}
/*

*/

