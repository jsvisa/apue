#ifndef M_SQLITE_H
#define M_SQLITE_H

#include <pthread.h>
#include "sqlite3.h"

/** SQLite3��װ�� */
class MSqlite {
	/** sqlite���ݿ��ļ�·�� */
	char path[256];
	/** ���ݿ�ṹ��ָ�� */
	sqlite3 *db;
	/** �������� */
	sqlite3_stmt *stmt;
	/** �Ƿ����� */
	bool connected;
	/** �Ƿ�ִ�й�һ�β�ѯ */
	bool hasStmt; 
	/** �Ƿ��ȡ��һ����¼*/
	bool hasRecord; 
	/** ���ݿ���ʻ����� */
	pthread_mutex_t dblock;
public:
	/** ���캯�� */
	MSqlite();
	/** �������� */
	~MSqlite();

	/** ���ӵ����ݿ� 
	 * path - ���ݿ��ļ�·��
	**/
	bool connect(const char *path);
	/** ִ��һ���ǲ�ѯ���
	 * sql - sql���
	 */
	bool exec(const char *sql);
	/** ִ��һ����ѯ��� 
	 * sql - sql���
	 */
	bool query(const char *sql);
	/** ��ȡһ����¼ */
	bool fetchRecord();
	/** ��ȡĳһ�е�ֵ 
	 * colNum - Ҫ��ȡ���к�
	 * buf - �е����ݴ����buf����Ϊ����
	 * maxSize - Ҫ��ȡ������ֽ��������ܳ���bufָ�����ڴ��С
	 */
	bool getColumnData(int colNum, char *buf, int maxSize);
	/** �ͷŲ�ѯ�Ľ���� */
	bool freeResult(); 
	/** �Ͽ����ݿ�����*/
	bool disconnect();
	/** �ж��Ƿ������������ݿ�*/
	bool isConnected();
	/** ��ȡ���ݿ�����Ĵ�����Ϣ */
	const char *getErrString();
	/** ��ʼ�� */
	void initLock();
	/** ���� */
	void lock();
	/** ���� */
	void unlock();
	/** ���Լ��� */
	bool tryLock();
	/** �ͷ��� */
	void destroyLock();
};

#endif

