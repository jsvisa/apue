#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "dbsys.h"
#include "shellsys.h"

void test(const char *buf) {
	char str[1024];
	checkBirth(buf);
//	trimString(buf);
//	printf("path:%s.\n", path);
//	readLine(str, 10);
//	checkMail(buf);
}

int doSession(int socket) {
	DBSYS dbsys;
	dbsys.socket = socket;

	strcpy(dbsys.path, "./student.db");

	if(initDBSYS(&dbsys) != 1) {
		fmtPrint(socket, "系统初始化失败，已退出系统！\n");
		return 0;
	}
	
	if(dbsysStart(&dbsys) != 0) {
		printf("系统错误，已退出!\n");
		return 0;
	}
}

int main(int argc, char **argv) {
	char ch;

	doSession(STDIN_FILENO);
	return 0;
}
