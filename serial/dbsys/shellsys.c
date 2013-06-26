#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <time.h>
#include "shellsys.h"

void trimString(char *str) {
	int pos;
	char *p;
	char *buf;
	int len;

	if(str == NULL || str[0] == 0) {
		return;
	}

	p = str + strlen(str) - 1;
	for(; (p>=str)&&(isspace(*p)); p--) {
		*p = 0;
	}
	len = strlen(str);
	for(p=str; p<str+len; p++) {
		if(!isspace(*p))
			break;
	}
	if(str != p) {
		memcpy(str, p, len-(p-str));
		str[len-(p-str)] = 0;
	}
}

int checkBirth(const char *birth) {
	int num;
	int year, mon, day;
	time_t t;
	struct tm tm;

	if(birth == NULL) {
		return 0;
	}
	
	time(&t);
	localtime_r(&t, &tm);
	num = sscanf(birth, "%d-%d-%d", &year, &mon, &day);
	if(num != 3) {
		printf("输入的日期格式不正确，请参考：2009-02-15\n");
		return 0;
	}
	if(year < 0) {
		printf("该同学不能出生于公元前\n");
		return 0;
	}
	if(year > tm.tm_year+1900) {
		printf("现在是%d年,该同学尚未出生\n", tm.tm_year+1900);
		return 0;
	}
	if(mon > 12 || mon < 1 || day > 31 || day < 1) {
		printf("输入的不是一个有效的日期:%s，请检查2009-02-15\n", birth);
		return 0;
	}
	return 1;
}

int checkGender(const char *buf) {
	if(buf == NULL)
		return 0;

	if(buf[0] != 'm' && buf[0] != 'f')
	   return 0;	
	return 1;
}

int checkMobile(const char *mobile) {
	char *p;

	if(mobile == NULL)
		return 0;
	if(strlen(mobile) < 8) {
		return 0;
	}
	for(p=(char *)mobile; *p!='\0'; p++) {
		if(*p > '9' || *p < '0')
			return 0;
	}
	return 1;
}

int checkMail(const char *mail) {
	char *p;

	if(mail == NULL)
		return 0;
	p = strchr(mail, '@');
	if(p == NULL) {
		return 0;
	}
	p = strchr(p, '.');
	if(p == NULL) {
		return 0;
	}
	return 1;
}

int readLine(int fd, char *buf, int maxSize) {
	int readed;
	char line[MAX_LINE];

	memset(line, 0, sizeof(line));
	if(buf == NULL) {
		return 0;
	}
	memset(line, 0, MAX_LINE);
	readed = read(fd, line, MAX_LINE);
	if(readed < 0) {
		return readed;
	}

	trimString(line);

	readed = strlen(line);
	if(maxSize < readed) {
		printf("您输入的字符数为%d个，超过最大长度限制：%d，超过的部分将被自动截除\n", readed, maxSize);
		readed = maxSize;
	}
	if(readed > 0)
		strcpy(buf, line);

	return readed;
}

int fmtPrint(int fd, char *fmt, ...) {
	int len;
	va_list ap;
	char line[1024*10];

	va_start(ap, fmt);
	len = vsnprintf(line, sizeof( line ), fmt, ap);
	va_end( ap );
//	printf("write socket:%s\n", line);
	return write(fd, line, strlen(line));
}
