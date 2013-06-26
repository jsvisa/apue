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
	if (rc != SQLITE_OK)
	{
		printf("open error %s\n", sqlite3_errmsg(db));
		return NULL;
	}
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
		printf("name=%s,address=%s\n", sqlite3_column_text(stmt, 0), 
				sqlite3_column_text(stmt, 1));
		rc = sqlite3_step(stmt);
	}

	i = sqlite3_data_count(stmt);
	printf("Data count:%d\n", i);
	sqlite3_finalize(stmt);
}

int main(int argc, char **argv)
{
	int i;
	char dbname[] = "/tmp/contact.db";
	sqlite3_stmt *stmt;
	sqlite3 *db;
	const char *tail;
	int rc;
	char sql[1024];
	char name[] = "helloqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm12345678900987654321poiuytrewqlkjhgfdsamnbvcxz1234567890poiuytrewqasdfghjklmnbvcxxxxxzasdfghjklpoiuytrewq12345678900987654321qwertyuiopoiuytrewqasdfghjkllkjhgfdsazxcvbnmmnbvcxzasdfghjkloiuytrewqsdfghjkliuytrewdfghjklkjhxcvbnmjkiuytrdcvbnjhgfdswertyuiolkjhvcbvcdsw3456789ijhgfdsdfghj";

	db = dbConnect(dbname, db);
	if (db == NULL)
	{
		return 1;	
	}

	char buf[1024];
	rc = sprintf(buf, "create table contact(id int auto_increment, name varchar(64), primary key(id))");
	dbExec(db, buf);
    sprintf(sql, "insert into contact(id, name) values(?1, $2)");
	//rc = dbExecStmt(db, sql);

	rc = sqlite3_prepare_v2(db, sql, 1023, &stmt, &tail);
	if(rc != SQLITE_OK) {
		printf("prepare:%s\n", sqlite3_errmsg(db));
		goto err_quit;
	}
	for(i=1; i<1000; i++) {
		rc = sqlite3_bind_int(stmt, 1, i);
		if(rc != SQLITE_OK) {
			printf("sqlite3_bind_int:%s\n", sqlite3_errmsg(db));
			goto err_quit;
		}

		//rc = sqlite3_bind_text(stmt, 2, name+(i-1)*3, 3, SQLITE_STATIC);
		rc = sqlite3_bind_text(stmt, 2, name+(i-1)*3, 3, NULL);

		if(rc != SQLITE_OK) {
			printf("sqlite3_bind_text:%s\n", sqlite3_errmsg(db));
			goto err_quit;
		}

		rc = sqlite3_step(stmt);
		//printf("step:%d\n", rc);

		if(rc != SQLITE_DONE) {
			printf("sqlite3_step:%s\n", sqlite3_errmsg(db));
			goto err_quit;
		}
		sqlite3_reset(stmt);
	}
err_quit:
	sqlite3_close(db);
	return 0;
}

