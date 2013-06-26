#ifndef DBSYS_H
#define DBSYS_H

#define MAX_CMD_SIZE 128
#define STUDENT_NAME_SIZE 16
#define STUDENT_GENDER_SIZE 1
#define STUDENT_BIRTH_SIZE 20
#define STUDENT_MOBILE_SIZE 20
#define STUDENT_MAILBOX_SIZE 64

#define ADD_MODE 1
#define EDIT_MODE 2


typedef struct Student {
	char name[STUDENT_NAME_SIZE];
	char gender;
	char birth[STUDENT_BIRTH_SIZE];
	char mobile[STUDENT_MOBILE_SIZE];
	char mailbox[STUDENT_MAILBOX_SIZE];
//	char className[32];
	unsigned int crc;
} STUDENT;

typedef struct StudentSysem {
	int socket; //fd-接收命令的文件描述符
	int dbFD; //数据库文件的文件描述符
	char path[128];//数据库文件的路径
	STUDENT student; //
	int recordStatus;
}DBSYS;

//初始化
int initDBSYS(DBSYS *dbsys);

int dbsysStart(DBSYS *dbsys);

int getRecord(DBSYS *dbsys, STUDENT *st);

int checkRecord(STUDENT *student);

int writeRecord(DBSYS *dbsys, STUDENT *student);

//列出文件里的所有记录
int doList(DBSYS *dbsys, const char *cmd);

int findByName(DBSYS *dbsys, const char *name, STUDENT *student);
#endif
