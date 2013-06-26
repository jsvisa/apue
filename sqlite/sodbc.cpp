#include <stdio.h>
#include <string.h>
#include <errmsg.h>
#include "sodbc.h"

/*
MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user, const char *passwd, const char *db, unsigned int port, const char *unix_socket, unsigned long client_flag)
client_flag usually is 0
if host is null,default is localhost
if port is not zero,use it to connect to mysql
*/

SODBC::SODBC() {
	isConnected = false;
	hasStmt = false;
	stmtFetched = false;
	mysql = NULL;
	results = NULL;
	fieldsNum = 0;
}

SODBC::~SODBC() {
	Disconnect();
}

// MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user, const char *passwd, const char *db, 
// unsigned int port, const char *unix_socket, unsigned long client_flag)
bool SODBC::Connect(const char* user, const char* passwd, const char *database, const char* host, int port) {
	mysql = mysql_init(NULL);
	if(mysql == NULL) 
		return false;

	if(mysql_real_connect(mysql, host, user, passwd, database, 0, NULL, 0) == NULL) {
//		printf("Connect to db failed\n");
		mysql_close(mysql);
		return false;
	}

	isConnected = true;
	return true;
}

void SODBC::Disconnect() {
	FreeResult();
	if(isConnected) {
#ifdef DEBUG
		printf("Disconnect\n");
#endif
		mysql_close(mysql);
		isConnected = false;
	}
	mysql = NULL;
}

bool SODBC::Exec(const char* sql) {
	int err;
	if(!isConnected)
		return false;

	if(mysql_query(mysql, sql) != 0) {
		err = mysql_errno(mysql);
		if(err == CR_SERVER_GONE_ERROR || err == CR_SERVER_LOST) { 
			Disconnect();
		}
		return false;
	}
	return true;
}

bool SODBC::Query(const char* sql) {
	int err;
	if(!isConnected)
		return false;
	FreeResult();

	if(mysql_query(mysql, sql) != 0) {
		err = mysql_errno(mysql);
		if(err == CR_SERVER_GONE_ERROR ||err == CR_SERVER_LOST) { 
			Disconnect();
		}
		return false;
	}

	results = mysql_store_result(mysql);
	if(results == NULL) {
		if(mysql_errno(mysql) == 0) {
			return true;
		}
		return false;
	}
	fieldsNum = mysql_num_fields(results);

	hasStmt = true;
	return true;
}

int SODBC::GetRowNum() {
	if(!hasStmt || results == NULL)
		return -1;
	return (int)mysql_num_rows(results);
}

bool SODBC::FetchRecord( ) {
	if(!hasStmt || results == NULL) 
		return false;
	if((record = mysql_fetch_row(results))) {
		hasFetchedRow = true;
		fieldLengths = mysql_fetch_lengths(results);
		/*
		printf("Field Lengths:");
		for(int i=0; i<fieldsNum; i++) 
			printf("%d ", fieldLengths[i]);
		printf("\n");
		*/
		return true;
	}
	return false;
}

void SODBC::FreeResult( ) {
	if(hasStmt) {
#ifdef DEBUG
		printf("Free result\n");
#endif
		if(results)
			mysql_free_result(results);
		results = NULL;
		hasStmt = false;
		hasFetchedRow = false;
		fieldsNum = 0;
	}
}

bool SODBC::GetColumnData(int columnNum, char* data, int size) {
	memset(data, 0, size);

	if(!hasFetchedRow)
		return false;
	if(columnNum >= (int)fieldsNum)
		return false;
//	printf("field %d length:%d\n", columnNum, fieldLengths[columnNum]);
	if(fieldLengths[columnNum] <= 0)
		return true;

	if((int)fieldLengths[columnNum] > size)
		memcpy(data, record[columnNum], size);
	else
		memcpy(data, record[columnNum], fieldLengths[columnNum]);

	return true;
}

int SODBC::GetFieldIndex(const char *name) {
	int i;
	int num_fields;
	MYSQL_FIELD *field;

	if(!hasStmt)
		return -1;
	num_fields = mysql_num_fields(results);
	for(i = 0; i < num_fields; i++) {
		field = mysql_fetch_field_direct(results, i);
		if(strcmp(name, field->name) == 0)
			return i;
//		printf("Field %u is %s\n", i, field->name);
	}
	return -1;
}

bool SODBC::GetColumnData(const char *column, char* data, int size) {
	memset(data, 0, size);
	int colNum;

	if(!hasFetchedRow)
		return false;

	colNum = GetFieldIndex(column);
	if(colNum < 0)
		return false;

	if((unsigned int)colNum >= fieldsNum)
		return false;
	if((int)fieldLengths[colNum] > size)
		memcpy(data, record[colNum], size);
	else
		memcpy(data, record[colNum], fieldLengths[colNum]);

	return true;
}

unsigned long SODBC::GetFieldNum() {
	if(hasStmt)
		return (unsigned long)mysql_num_fields(results);
	return 0;
}

bool SODBC::GetFieldName(int columnNum , char* data, int size) {
	MYSQL_FIELD *field;

	memset(data, 0, size);
	if(!hasStmt)
		return false;
	if(columnNum >= (int)fieldsNum)
		return false;

	field = mysql_fetch_field_direct(results, columnNum);
	strncpy(data, field->name, size);
	return true;
}

int SODBC::GetAffectedRows() {
	return (int)mysql_affected_rows(mysql);
}

bool SODBC::Available() {
	return isConnected;
}

unsigned int SODBC::GetErrorCode() {
	return mysql_errno(mysql);
}

const char *SODBC::GetErrorString() {
	return mysql_error(mysql);
}

int SODBC::Ping() {
	return mysql_ping(mysql);
}
