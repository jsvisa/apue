#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>


int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

sqlite3 *dbConnect(char *db_name, sqlite3 *db) {
	int rc;
	rc = sqlite3_open(db_name, &db);
	return db;
}

int dbExecStmt(sqlite3 *db, char *sql)
{
	int rc;
	char *zErrMsg = 0;
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		exit(1);
	}
	return 0;
}

void dbExec(sqlite3 *db, char *sql)
{
	sqlite3_stmt *stmt;
	const char *tail;
	int i;
	int ncols;
	int rc;

	rc = sqlite3_prepare(db, sql, strlen(sql), &stmt, &tail);
	i = sqlite3_data_count(stmt);
	printf("Data count:%d\n", i);
	i= sqlite3_column_count(stmt);
	printf("Column count:%d\n", i);
	char *name, *address;
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "sql error:%s\n", sqlite3_errmsg(db));
	}
	//从结果集里取出一条记录
	rc = sqlite3_step(stmt);
	//获取结果集的列数
	ncols = sqlite3_column_count(stmt);
	while (rc == SQLITE_ROW)
	{
		printf("rc=%d\n", rc);
			//打印某一列的值
		printf(" name=%s,address=%s\n", sqlite3_column_text(stmt, 0), 
				sqlite3_column_text(stmt, 1));
		rc = sqlite3_step(stmt);
	}

	i = sqlite3_data_count(stmt);
	printf("Data count:%d\n", i);
	sqlite3_finalize(stmt);
}


int main(int argc, char **argv)
{
	char dbname[] = "/tmp/contact.db";
	sqlite3 *db;

	char sql[1024] = "select name, address from contact";
	int rc;
	db = dbConnect(dbname, db);
	//rc = dbExecStmt(db, sql);

	char buf[1024];
	sprintf(buf, "create table contact(id int auto_increment, name varchar(64), primary key(id))");
	dbExec(db, buf);
	dbExec(db, sql);

	sqlite3_close(db);
	return 0;
}

