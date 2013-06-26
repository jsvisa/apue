#ifndef SODBC_H
#define SODBC_H

#include <mysql.h>
#include <stdio.h>

//#define DEBUG 1

class SODBC {
	MYSQL *mysql;
	MYSQL_RES *results;
	MYSQL_ROW record;

	bool hasStmt; //是否执行过一条查询语句
	bool hasFetchedRow; //是否获取一条记录
	bool stmtFetched; //是否已取出结果集
	bool isConnected;//是否连接
	unsigned int fieldsNum; //某条查询语句返回列的个数
	long unsigned int *fieldLengths; //某一列的长度

public:
	SODBC();

	~SODBC();

	void Disconnect();

	//连接数据库，user-使用的用户名，passwd-密码，database-数据库名字, 
	//host-连接到的主机地址,为NULL时表示连接本机, port-为0表示默认
	bool Connect(const char* user, const char* passwd, 
			const char *database, const char* host=NULL, int port=0);

	//指行一条非查询语句：update,insert,drop table,create table....
	bool Exec(const char* sql);

	//执行一条查询语句：select
	bool Query(const char* sql);
	
//	bool fetch( );

//	void defetch( );

	//获取结果集中某一列的值,cowNum-列号，data-数据返回存放在data里，size-最大的长度
	bool GetColumnData(int cowNum, char* data, int size);

	//根据列的名字来获取结果
	bool GetColumnData(const char *colName, char *data, int size);

	//获取失败错误码
	unsigned int GetErrorCode();

	//获取失败的字符串
	const char *GetErrorString();

	//从结果集中取出一条记录
	bool FetchRecord( );

	//释放结果集
	void FreeResult( );

	unsigned long GetFieldNum();

	bool GetFieldName(int columnNum , char *data , int size);

	//获取结果集的行数
	int GetRowNum();

	//获取上次UPDATE、DELETE或INSERT查询更改／删除／插入的行数。
	int GetAffectedRows();

	bool Available();

	int GetFieldIndex(const char *name);

	int Ping();
	/*
	bool getFieldType(int columnNum , char* fieldType, int size);
	bool setTransactionIsolation(int level);
	bool rollback();
	bool commit();
	bool setAutoCommit(bool enable);
	bool allocHdbc();
	void setDBMSName(const char* name);
	const char* getDBMSName();
	*/
};

#endif

