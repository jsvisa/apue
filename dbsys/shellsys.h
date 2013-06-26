#ifndef DBSYS_SHELL_H
#define DBSYS_SHELL_H

#define MAX_LINE 1024

/** �ӿ���̨��������,��󲻳���maxSize�ֽ� 
 *  ���شӿ���̨���ص��ֽ�����-1��ʾ��ȡʧ��
 */
int readLine(int sock, char *buf, int maxSize);

/* ȥ���ַ�����ͷ�Ŀո����
 * �����޸�ԭ�ַ���������
 */
void trimString(char *str);

/* ��������Ƿ�Ϸ� 
 * ����1��ʾ�ǺϷ������ڣ�����0��ʾ�Ƿ�������
 */
int checkBirth(const char *birth);

/* ����Ա��Ƿ�Ϸ� 
 * ����1��ʾ�Ϸ����Ա��ǣ�����Ϊ0
 */
int checkGender(const char *buf);

/* ����ʼ���ַ�Ƿ�Ϸ� 
 * ����1��ʾ�Ϸ����ʼ���ַ��0��ʾ���ϸ�ʽ���ʼ���ַ
 */
int checkMail(const char *mail);

int checkMobile(const char *mobile);

int fmtPrint(int sock, char *fmt, ...);
#endif
