#include "msqlite.h"
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

void *testThread(void *args) {
	MSqlite *sqlite = (MSqlite *)args;
	int i;
	char buf[1024];

	pthread_detach(pthread_self());
	//sleep(1);
	sqlite->lock();
	printf("[Thread]lock success\n");

	for (i = 1; i < 1000; i++)
	{
		memset(buf, 0, 1024);
		sprintf(buf, "insert into student values(%d, 'Fang%05d', 1, '2012-12-21', 1)", i, i);
		if (!sqlite->exec(buf))
		{
			printf("exec error...\n");	
			sqlite->unlock();
			return NULL;
		}
	}
	
	sqlite->unlock();
	return NULL;
}

int main(int argc, char **argv) {
	MSqlite sqlite;
	pthread_t tid;
	char id[255];
	char name[255];
	char address[255];

	if(!sqlite.connect("./student.db")) {
		printf("[Main]:Connect to db failed\n");
		return 1;
	}

	sqlite.initLock();
	pthread_create(&tid, NULL, testThread, (void *)&sqlite);

	sleep(1);
	sqlite.lock();
	if(!sqlite.query("select * from student WHERE id < 5")) {
		printf("[Main]Query failed:%s\n", sqlite.getErrString());
		return 0;
	}
	printf("[Main]Query success\n");
	sleep(3);
	while(sqlite.fetchRecord()) {
		sqlite.getColumnData(0, id, 16);
		sqlite.getColumnData(1, name, 32);
		sqlite.getColumnData(2, address, 250);
		printf("[Main]:%s,%s,%s\n", id, name, address);
	}

	sqlite.freeResult();
	printf("return ok\n");
	sqlite.unlock();
	sleep(5);
	return 0;
}

