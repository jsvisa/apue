#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

int calltimes=0;
static int funccc(void* arg, int argc, char** argv, char** azColName)
{
	int i;

//	printf("arg=%s\n", (char *)arg);
	if(calltimes == 0) {
		for(i=0; i<argc; i++) {
			printf("%s\t", azColName[i]);
		}
	}
	printf("\n");
	calltimes++;

	for(i=0; i<argc; i++) {
		printf("%s\t", argv[i] ? argv[i] : "NULL"); //打印出查询
	}
	return 0;
}

int main(int argc, char **argv) {
	sqlite3 *db = NULL;
	int rc;
	int i;
	char buf[1024];
	const char *tail;
	char str[] = "in callback function";
	sqlite3_stmt *stmt;

	rc = sqlite3_open("student.db", &db);
	if(rc != SQLITE_OK) {
		printf("Open sql failed:%s\n", sqlite3_errmsg(db));
		return 1;
	}

/*	
	sprintf(buf, "delete from student where id>10");
	rc = sqlite3_exec(db, buf, NULL, NULL, NULL);
	if(rc  != SQLITE_OK) {
		printf("Exec sql failed:%s\n", sqlite3_errmsg(db));
	}
*/

	for(i=0; i<1000; i++) {
		sprintf(buf, "INSERT INTO student VALUES(%d, 'Fang%05d', 1, '2012-12-21', 1)", i, i);
		if(sqlite3_exec(db, buf, NULL, NULL, NULL) != SQLITE_OK) {
			printf("Exec sql failed:%s\n", buf);
		}
	}

/*
	rc = sqlite3_exec(db, "SELECT id,name, birth FROM student WHERE id<30",
			funccc, (void*)str, NULL);
	if(rc != SQLITE_OK) {
		printf("Exec sql failed:%s\n", sqlite3_errmsg(db));
	}
*/
	
	//printf("rc=%d\n", rc);
	
	/*
	char *tmp = "update student set name='mmtest' where id=1";
	sqlite3_prepare(db, tmp, strlen(tmp), &stmt, &tail);
	rc = sqlite3_step(stmt);
	if(rc != SQLITE_DONE) {
		printf("step failed :%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return 1;
	}

	sqlite3_finalize(stmt);
	*/
	
	/*
	char *tmp1 = "SELECT id,name,gender,birth,class_id FROM student where id>30 and id < 40; select * from student";
	if(sqlite3_prepare(db, tmp1, strlen(tmp1)+1, &stmt, &tail) != SQLITE_OK) {
		printf("prepare failed:%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return 1;
	}

	printf("tail=%s\n", tail);

	while(1) {
		rc = sqlite3_step(stmt);
		if(rc != SQLITE_ROW) {
			if(rc != SQLITE_DONE) {
				printf("Error:%s\n", sqlite3_errmsg(db));
			}
			break;
		}
		
		printf("Line:%d,%s,%s,%s,%s\n", 
				sqlite3_column_int(stmt, 0), 
				sqlite3_column_text(stmt, 1), 
				sqlite3_column_text(stmt, 2), 
				sqlite3_column_text(stmt, 3), 
				sqlite3_column_text(stmt, 4));
	}

	sqlite3_finalize(stmt);
	*/

	sqlite3_close(db);

	printf("number %d callback\n", calltimes);  //打印出回调的次数
	return 0;
}

