#ifndef DBSYS_SHELL_H
#define DBSYS_SHELL_H

#define MAX_LINE 1024

/** 从控制台读入数据,最大不超过maxSize字节 
 *  返回从控制台读回的字节数，-1表示读取失败
 */
int readLine(int sock, char *buf, int maxSize);

/* 去掉字符串两头的空格符号
 * 将会修改原字符串的内容
 */
void trimString(char *str);

/* 检查日期是否合法 
 * 返回1表示是合法的日期，返回0表示非法的日期
 */
int checkBirth(const char *birth);

/* 检查性别是否合法 
 * 返回1表示合法的性别标记，否则为0
 */
int checkGender(const char *buf);

/* 检查邮件地址是否合法 
 * 返回1表示合法的邮件地址，0表示不合格式的邮件地址
 */
int checkMail(const char *mail);

int checkMobile(const char *mobile);

int fmtPrint(int sock, char *fmt, ...);
#endif
