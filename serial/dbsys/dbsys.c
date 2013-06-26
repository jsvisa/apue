#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include "dbsys.h"
#include "crc16.h"
#include "shellsys.h"
#include "filelock.h"

void printRecord(DBSYS *dbsys, STUDENT *student) {
	fmtPrint(dbsys->socket, "%-10s%-10s%-20s%-20s%s\n", student->name, student->gender=='m'?"男":"女", 
			student->birth, student->mobile, student->mailbox);
}

int initDBSYS(DBSYS *dbsys) {
	int ret;
	int size;
	int count = 0;

	dbsys->dbFD = open(dbsys->path, O_CREAT|O_RDWR, S_IRWXU);
	if(dbsys->dbFD == -1) {
		fmtPrint(dbsys->socket, "打开数据库文件%s失败:%s\n", dbsys->path, strerror(errno));
		return 0;
	}
	if(doList(dbsys, NULL) < 0) {
		return -1;
	}
	return 1;
}

int getRecord(DBSYS *dbsys, STUDENT *student) {
	int size;

	//读取记录
	size = read(dbsys->dbFD, student, sizeof(STUDENT));
	if(size > 0 && size != sizeof(STUDENT)) {
		fmtPrint(dbsys->socket, "读记录失败，数据库文件可能已损坏\n");
		lseek(dbsys->dbFD, -size, SEEK_END);
		return 0;
	}

	//已到文件尾
	if(size == 0)
		return 0;

	//CRC数据校验
//	fmtPrint(dbsys->socket, "CRC:%d\n", dbsys->student.crc);
	if(student->crc != CRC16((unsigned char *)student, sizeof(STUDENT)-4)) {
		char choice[2];
		fmtPrint(dbsys->socket, "CRC校验错误，数据库文件可能已损坏\n");
		fmtPrint(dbsys->socket, "姓名      性别      生日                手机                邮箱\n");
		printRecord(dbsys, student);
		fmtPrint(dbsys->socket, "是否现在修复此记录(Y/N)?");
		readLine(dbsys->socket, choice, 1);
		if(choice[0] != 0 && choice[0] != 'y' && choice[0] != 'Y') {
			fmtPrint(dbsys->socket, "放弃修复记录\n");
		}
		else {
			//编辑记录
			return EditRecord(dbsys, student);
		}
	}
	return 1;
}

int fmtPrintRecord(DBSYS *dbsys, STUDENT *student) {
	/*
	lseek(dbsys->dbFD, 0, SEEK_END);
	memset(student, 0, sizeof(STUDENT));
	strcpy(student->name, "fang");
	strcpy(student->birth, "1001-01-01");
	strcpy(student->mobile, "13333111");
	strcpy(student->mailbox, "fang@sino.com");
	student->gender = 1;
	*/
	student->crc = CRC16((unsigned char *)student, sizeof(STUDENT)-4);
//	fmtPrint(dbsys->socket, "CRC:%d\n", student->crc);
	if(write(dbsys->dbFD, student, sizeof(STUDENT)) != sizeof(STUDENT)) {
		fmtPrint(dbsys->socket, "写入数据库记录失败\n");
		return -1;
	}
	return 1;
}

int checkRecord(STUDENT *student) {
	if(student->crc == CRC16((unsigned char *)student, sizeof(STUDENT)-4))
		return 1;
	return 0;
}

int findByName(DBSYS *dbsys, const char *name, STUDENT *student) {
	int ret;

	lseek(dbsys->dbFD, 0, SEEK_SET);
	while((ret = getRecord(dbsys, student)) > 0) {
		if(strcmp(name, student->name) == 0)
			break;
	}

	return ret;
}

// get xxxx
int doGet(DBSYS *dbsys, const char *cmd) {
	int ret;
	char name[128];

	strcpy(name, cmd+4);
	trimString(name);
	
	readLock(dbsys->dbFD, 0, SEEK_SET, 0);
	ret = findByName(dbsys, name, &(dbsys->student));
	unlock(dbsys->dbFD, 0, SEEK_SET, 0);
	if(ret > 0) {
		fmtPrint(dbsys->socket, "姓名      性别      生日                手机                邮箱\n");
		printRecord(dbsys, &(dbsys->student));
	}
	else if(ret == 0) {
		fmtPrint(dbsys->socket, "未找到记录：\"%s\"\n", name);
	}
	else {
		fmtPrint(dbsys->socket, "数据库访问错误:%s\n", strerror(errno));
		return -1;
	}

	return 1;
}

int printPrompt(DBSYS *dbsys, char *buf) {
	if(buf != 0) {
		return fmtPrint(dbsys->socket, buf, strlen(buf));
	}
	return 0;
}

int getName(DBSYS *dbsys, char *name, int mode) {
	int size;
	char buf[1024];

	if(mode == EDIT_MODE) 
		sprintf(buf, "请输入用户名(%s)：", name);
	else
		sprintf(buf, "请输入用户名：");

	printPrompt(dbsys, buf);
	if((size = readLine(dbsys->socket, buf, STUDENT_NAME_SIZE-1)) < 0) {
		fmtPrint(dbsys->socket, "取消操作。\n");
		return size;
	}
	else if(size == 0) {
		if(mode == EDIT_MODE) 
			return 1;
		else
			return 0;
	}
	if(mode != EDIT_MODE) {
		readLock(dbsys->dbFD, 0, SEEK_SET, 0);
		if(findByName(dbsys, buf, &(dbsys->student))) {
			fmtPrint(dbsys->socket, "学生(%s)已存在。\n", buf);
			unlock(dbsys->dbFD, 0, SEEK_SET, 0);
			return 0;
		}
		unlock(dbsys->dbFD, 0, SEEK_SET, 0);
	}

	strcpy(name, buf);
	return 1;
}

int getGender(DBSYS *dbsys, char *gender, int mode) {
	int size;
	char buf[128];
	if(mode == EDIT_MODE) 
		sprintf(buf, "请输入性别，m-男，f-女，请输入m或f：(%c)", *gender);
	else
		sprintf(buf, "请输入性别，m-男，f-女，请输入m或f：");

	printPrompt(dbsys, buf);
	size = readLine(dbsys->socket, buf, STUDENT_GENDER_SIZE);
	if(size < 0) {
		fmtPrint(dbsys->socket, "取消操作。\n");
		return size;
	}
	else if(size == 0) {
		if(mode == EDIT_MODE)
			return 1;
		else 
			return 0;
	}
	if(checkGender(buf)) {
		*gender = buf[0];
		return 1;
	}
	fmtPrint(dbsys->socket, "请输入m或f\n");
	return 0;
}

int getMobile(DBSYS *dbsys, char *mobile, int mode) {
	int size;
	char buf[128];

	if(mode == EDIT_MODE) 
		sprintf(buf, "请输入手机号：(%s)", mobile);
	else
		sprintf(buf, "请输入手机号：");

	printPrompt(dbsys, buf);
	size = readLine(dbsys->socket, buf, STUDENT_MOBILE_SIZE-1);
	if(size < 0) {
		fmtPrint(dbsys->socket, "取消操作。\n");
		return size;
	}
	else if(size == 0) {
		if(mode == EDIT_MODE)
			return 1;
		else 
			return 0;
	}
	if(checkMobile(buf)) {
		strcpy(mobile, buf);
		return 1;
	}
	else {
		fmtPrint(dbsys->socket, "请输入正确的手机号\n");
		return 0;
	}
}

int getBirth(DBSYS *dbsys, char *birth, int mode) {
	int size;
	char buf[128];

	if(mode == EDIT_MODE) 
		sprintf(buf, "请输入出生日期：(%s)", birth);
	else
		sprintf(buf, "请输入出生日期：");

	printPrompt(dbsys, buf);
	size = readLine(dbsys->socket, buf, STUDENT_BIRTH_SIZE-1);
	if(size < 0) {
		fmtPrint(dbsys->socket, "取消操作。\n");
		return size;
	}
	else if(size == 0) {
		if(mode == EDIT_MODE)
			return 1;
		else 
			return 0;
	}
	if(checkBirth(buf)) {
		strcpy(birth, buf);
		return 1;
	}
	else {
		fmtPrint(dbsys->socket, "请输入正确的日期\n");
		return 0;
	}
}

int getMail(DBSYS *dbsys, char *mail, int mode) {
	int size;
	char buf[128];

	if(mode == EDIT_MODE) 
		sprintf(buf, "请输入邮箱：(%s)", mail);
	else
		sprintf(buf, "请输入邮箱：");

	printPrompt(dbsys, buf);
	size = readLine(dbsys->socket, buf, STUDENT_MAILBOX_SIZE-1);
	if(size < 0) {
		fmtPrint(dbsys->socket, "取消操作。\n");
		return size;
	}
	else if(size == 0) {
		if(mode == EDIT_MODE)
			return 1;
		else 
			return 0;
	}
	if(checkMail(buf)) {
		strcpy(mail, buf);
		return 1;
	}
	else {
		fmtPrint(dbsys->socket, "请输入正确的邮箱\n");
		return 0;
	}
}

int doAdd(DBSYS *dbsys, const char *cmd) {
	int size;
	char buf[128];
	STUDENT student;

	if((size = getName(dbsys, student.name, ADD_MODE)) <=0) {
		return size;
	}
		
	if((size = getGender(dbsys, &student.gender, ADD_MODE)) <= 0) {
		return size;
	}

	if((size = getBirth(dbsys, student.birth, ADD_MODE)) <= 0) {
		return size;
	}

	if((size = getMobile(dbsys, student.mobile, ADD_MODE)) <= 0) {
		return size;
	}

	if((size = getMail(dbsys, student.mailbox, ADD_MODE)) <= 0) {
		return size;
	}

	fmtPrint(dbsys->socket, "姓名      性别      生日                手机                邮箱\n");
	printRecord(dbsys, &student);
	//查找到第一个记录为空的位置或到文件尾
	lseek(dbsys->dbFD, 0, SEEK_SET);

	writeLock(dbsys->dbFD, 0, SEEK_SET, 0);
	while(1) {
		size = getRecord(dbsys, &(dbsys->student));
		if(size > 0) {
			if(dbsys->student.name[0] == 0) {
				lseek(dbsys->dbFD, -sizeof(STUDENT), SEEK_CUR);
				break;
			}
		}
		else if(size == 0)
			break;
		else {
			fmtPrint(dbsys->socket, "访问数据库文件错误:%s\n", strerror(errno));
			unlock(dbsys->dbFD, 0, SEEK_SET, 0);
			return size;
		}
	}
	if(fmtPrintRecord(dbsys, &(student)) != 1) {
		fmtPrint(dbsys->socket, "添加用户到数据库失败：%s\n", strerror(errno));
		unlock(dbsys->dbFD, 0, SEEK_SET, 0);
		return -1;
	}
	unlock(dbsys->dbFD, 0, SEEK_SET, 0);

	printPrompt(dbsys, "添加用户到数据库成功！\n");
	return 1;
}

int doDel(DBSYS *dbsys, const char *cmd) {
	int ret;
	char name[128];
	char choice[2];

	strcpy(name, cmd+3);
	trimString(name);

	//找到记录
	readLock(dbsys->dbFD, 0, SEEK_SET, 0);
	ret = findByName(dbsys, name, &(dbsys->student));
	unlock(dbsys->dbFD, 0, SEEK_SET, 0);

	if(ret > 0) {
		//清空该记录
		printPrompt(dbsys, "确认删除用户？(Y/N) Y:");
		readLine(dbsys->socket, choice, 1);
		if(choice[0] != 0 && choice[0] != 'y' && choice[0] != 'Y') {
			fmtPrint(dbsys->socket, "未删除用户\n");
			return 0;
		}
		memset(&(dbsys->student), 0, sizeof(STUDENT));
		writeLock(dbsys->dbFD, 0, SEEK_SET, 0);
		lseek(dbsys->dbFD, -sizeof(STUDENT), SEEK_CUR);
		if(fmtPrintRecord(dbsys, &(dbsys->student)) > 0) {
			fmtPrint(dbsys->socket, "删除用户\"%s\"成功！\n", name);
			unlock(dbsys->dbFD, 0, SEEK_SET, 0);
			return 1;
		}
		unlock(dbsys->dbFD, 0, SEEK_SET, 0);
		fmtPrint(dbsys->socket, "删除用户\"%s\"失败！\n", name);
		return -1;
	}
	else if(ret == 0){
		fmtPrint(dbsys->socket, "未找到记录：%s\n", name);
		return 1;
	}
	else {
		fmtPrint(dbsys->socket, "访问数据库文件错误!\n", strerror(errno));
		return ret;
	}
}

int EditRecord(DBSYS *dbsys, STUDENT *student) {
	int size;
	char name[128];
	char choice[2];


	if((size = getName(dbsys, student->name, EDIT_MODE)) <=0) 
		return size;
	if((size = getGender(dbsys, &student->gender, EDIT_MODE)) <= 0) 
		return size;
	if((size = getBirth(dbsys, student->birth, EDIT_MODE)) <= 0) 
		return size;
	if((size = getMobile(dbsys, student->mobile, EDIT_MODE)) <= 0) 
		return size;
	if((size = getMail(dbsys, student->mailbox, EDIT_MODE)) <= 0) 
		return size;
	fmtPrint(dbsys->socket, "姓名      性别      生日                手机                邮箱\n");
	printRecord(dbsys, student);

	printPrompt(dbsys, "确认保存修改吗？(Y/N) Y:");
	readLine(dbsys->socket, choice, 1);
	if(choice[0] != 0 && choice[0] != 'y' && choice[0] != 'Y') {
		fmtPrint(dbsys->socket, "未保存修改\n");
		return 0;
	}

	writeLock(dbsys->dbFD, 0, SEEK_SET, 0);
	size = lseek(dbsys->dbFD, -sizeof(STUDENT), SEEK_CUR);
	if(fmtPrintRecord(dbsys, student) > 0) {
		unlock(dbsys->dbFD, 0, SEEK_SET, 0);
		fmtPrint(dbsys->socket, "保存用户\"%s\"数据成功！\n", student->name);
		return 1;
	}
	unlock(dbsys->dbFD, 0, SEEK_SET, 0);
	fmtPrint(dbsys->socket, "保存用户\"%s\"数据失败！\n", student->name);

	return -1;
}
int doEdit(DBSYS *dbsys, const char *cmd) {
	int size;
	char name[128];
	char choice[2];
	STUDENT student;

	strcpy(name, cmd+4);
	trimString(name);

	readLock(dbsys->dbFD, 0, SEEK_SET, 0);
	size = findByName(dbsys, name, &student);
	unlock(dbsys->dbFD, 0, SEEK_SET, 0);

	if(size == 0) {
		fmtPrint(dbsys->socket, "未找到记录：%s\n", name);
		return 0;
	}
	else if(size < 0) {
		fmtPrint(dbsys->socket, "访问数据库文件错误\n");
		return size;
	}

	fmtPrint(dbsys->socket, "姓名      性别      生日                手机                邮箱\n");
	printRecord(dbsys, &student);
	/*
	if((size = getName(dbsys, student.name, EDIT_MODE)) <=0) 
		return size;
	*/
	//strcpy(student.name, name);
	if((size = getGender(dbsys, &student.gender, EDIT_MODE)) <= 0) 
		return size;
	if((size = getBirth(dbsys, student.birth, EDIT_MODE)) <= 0) 
		return size;
	if((size = getMobile(dbsys, student.mobile, EDIT_MODE)) <= 0) 
		return size;
	if((size = getMail(dbsys, student.mailbox, EDIT_MODE)) <= 0) 
		return size;
	fmtPrint(dbsys->socket, "姓名      性别      生日                手机                邮箱\n");
	printRecord(dbsys, &student);

	printPrompt(dbsys, "确认保存修改吗？(Y/N) Y:");
	readLine(dbsys->socket, choice, 1);
	if(choice[0] != 0 && choice[0] != 'y' && choice[0] != 'Y') {
		fmtPrint(dbsys->socket, "未保存修改\n");
		return 0;
	}

	writeLock(dbsys->dbFD, 0, SEEK_SET, 0);
	//size = findByName(dbsys, name, &student);
	size = lseek(dbsys->dbFD, -sizeof(STUDENT), SEEK_CUR);
	if(fmtPrintRecord(dbsys, &student) > 0) {
		unlock(dbsys->dbFD, 0, SEEK_SET, 0);
		fmtPrint(dbsys->socket, "保存用户\"%s\"数据成功！\n", name);
		return 1;
	}
	unlock(dbsys->dbFD, 0, SEEK_SET, 0);
	fmtPrint(dbsys->socket, "保存用户\"%s\"数据失败！\n", name);

	return -1;
}

int doList(DBSYS *dbsys, const char *cmd) {
	int ret;
	STUDENT student;

	lseek(dbsys->dbFD, 0, SEEK_SET);
	fmtPrint(dbsys->socket, "姓名      性别      生日                手机                邮箱\n");

	readLock(dbsys->dbFD, 0, SEEK_SET, 0);
	while(1) {
		ret = getRecord(dbsys, &student);

		if(ret > 0) {
			if(student.name[0] == 0)
				continue;
			printRecord(dbsys, &student);
		}
		else if(ret < 0) {
			fmtPrint(dbsys->socket, "即将退出系统\n");
			break;
		}
		else {
			break;
		}
	}
	unlock(dbsys->dbFD, 0, SEEK_SET, 0);
	return ret;
}

int doCmd(DBSYS *dbsys, const char *cmd) {
	int ret;

	if(strncmp(cmd, "get", 3) == 0) {
		ret = doGet(dbsys, cmd);
	}
	else if(strncmp(cmd, "list", 4) == 0) {
		ret = doList(dbsys, cmd);
	}
	else if(strncmp(cmd, "add", 3) == 0) {
		ret = doAdd(dbsys, cmd);
	}
	else if(strncmp(cmd, "del", 3) == 0) {
		ret = doDel(dbsys, cmd);
	}
	else if(strncmp(cmd, "edit", 4) == 0) {
		ret = doEdit(dbsys, cmd);
	}
	else if(strncmp(cmd, "quit", 4) == 0) {
		fmtPrint(dbsys->socket, "欢迎下次使用本系统!\n");
		return -1;
	}
	else {
		fmtPrint(dbsys->socket, "未知指令!\n");
		ret = 1;
	}
	return ret;
}

int dbsysStart(DBSYS *dbsys) {
	int size;
	char cmd[MAX_CMD_SIZE+1];

	while(1) {
		fmtPrint(dbsys->socket, "dbsys>", strlen("dbsys>"));
		size = readLine(dbsys->socket, cmd, MAX_CMD_SIZE);
		if(size < 0) {
			fmtPrint(dbsys->socket, "IO错误\n");
			return -1;
		}
//		fmtPrint(dbsys->socket, "Get Cmd:%s\n", cmd);
		if(doCmd(dbsys, cmd) < 0) {
			break;
		}
	}
	close(dbsys->dbFD);
	return 0;
}
