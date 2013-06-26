#ifndef M_SQLITE_H
#define M_SQLITE_H

#include <pthread.h>
#include "sqlite3.h"

/** SQLite3封装类 */
class MSqlite {
	/** sqlite数据库文件路径 */
	char path[256];
	/** 数据库结构体指针 */
	sqlite3 *db;
	/** 编译结果集 */
	sqlite3_stmt *stmt;
	/** 是否连接 */
	bool connected;
	/** 是否执行过一次查询 */
	bool hasStmt; 
	/** 是否获取过一条记录*/
	bool hasRecord; 
	/** 数据库访问互斥锁 */
	pthread_mutex_t dblock;
public:
	/** 构造函数 */
	MSqlite();
	/** 析构函数 */
	~MSqlite();

	/** 连接到数据库 
	 * path - 数据库文件路径
	**/
	bool connect(const char *path);
	/** 执行一条非查询语句
	 * sql - sql语句
	 */
	bool exec(const char *sql);
	/** 执行一条查询语句 
	 * sql - sql语句
	 */
	bool query(const char *sql);
	/** 获取一条记录 */
	bool fetchRecord();
	/** 获取某一列的值 
	 * colNum - 要获取的列号
	 * buf - 列的内容存放在buf内作为返回
	 * maxSize - 要获取的最大字节数，不能超过buf指定的内存大小
	 */
	bool getColumnData(int colNum, char *buf, int maxSize);
	/** 释放查询的结果集 */
	bool freeResult(); 
	/** 断开数据库连接*/
	bool disconnect();
	/** 判断是否已连接上数据库*/
	bool isConnected();
	/** 获取数据库操作的错误信息 */
	const char *getErrString();
	/** 初始化 */
	void initLock();
	/** 加锁 */
	void lock();
	/** 解锁 */
	void unlock();
	/** 尝试加锁 */
	bool tryLock();
	/** 释放锁 */
	void destroyLock();
};

#endif

