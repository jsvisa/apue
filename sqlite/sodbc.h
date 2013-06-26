#ifndef SODBC_H
#define SODBC_H

#include <mysql.h>
#include <stdio.h>

//#define DEBUG 1

class SODBC {
	MYSQL *mysql;
	MYSQL_RES *results;
	MYSQL_ROW record;

	bool hasStmt; //�Ƿ�ִ�й�һ����ѯ���
	bool hasFetchedRow; //�Ƿ��ȡһ����¼
	bool stmtFetched; //�Ƿ���ȡ�������
	bool isConnected;//�Ƿ�����
	unsigned int fieldsNum; //ĳ����ѯ��䷵���еĸ���
	long unsigned int *fieldLengths; //ĳһ�еĳ���

public:
	SODBC();

	~SODBC();

	void Disconnect();

	//�������ݿ⣬user-ʹ�õ��û�����passwd-���룬database-���ݿ�����, 
	//host-���ӵ���������ַ,ΪNULLʱ��ʾ���ӱ���, port-Ϊ0��ʾĬ��
	bool Connect(const char* user, const char* passwd, 
			const char *database, const char* host=NULL, int port=0);

	//ָ��һ���ǲ�ѯ��䣺update,insert,drop table,create table....
	bool Exec(const char* sql);

	//ִ��һ����ѯ��䣺select
	bool Query(const char* sql);
	
//	bool fetch( );

//	void defetch( );

	//��ȡ�������ĳһ�е�ֵ,cowNum-�кţ�data-���ݷ��ش����data�size-���ĳ���
	bool GetColumnData(int cowNum, char* data, int size);

	//�����е���������ȡ���
	bool GetColumnData(const char *colName, char *data, int size);

	//��ȡʧ�ܴ�����
	unsigned int GetErrorCode();

	//��ȡʧ�ܵ��ַ���
	const char *GetErrorString();

	//�ӽ������ȡ��һ����¼
	bool FetchRecord( );

	//�ͷŽ����
	void FreeResult( );

	unsigned long GetFieldNum();

	bool GetFieldName(int columnNum , char *data , int size);

	//��ȡ�����������
	int GetRowNum();

	//��ȡ�ϴ�UPDATE��DELETE��INSERT��ѯ���ģ�ɾ���������������
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

