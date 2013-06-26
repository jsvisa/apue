#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

typedef struct student {
	char name[20];
	char gender;
	char birth[20];
	char mobile[20];
	char mail[64];
}STUDENT;

int main(int argc, char **argv) {
	/*
	int fd;
	
	STUDENT s;
	memset(&s, 0, sizeof(STUDENT));
	strcpy(s.name, "fang");
	s.gender = '1';
	strncpy(s.birth, "2000-01-01", 20);
	strncpy(s.mobile, "15822222200000000000000", 19);
	strncpy(s.mail, "sss@aaa.com", 64);

	fd = open("./student.db", O_CREAT|O_RDWR, S_IRWXU);
	if(fd < 0) {
		printf("Create file failed\n");
		return 1;
	}
	
	printf("sizeof struct:%d\n", sizeof(STUDENT));
	write(fd, &s, sizeof(STUDENT)); 
	strcpy(s.name, "ttttt");
	write(fd, &s, sizeof(STUDENT)); 

	close(fd);
	*/
	int fd;
	
	STUDENT s;
	fd = open("./student.db", O_CREAT|O_RDWR, S_IRWXU);
	if(fd < 0) {
		printf("Create file failed\n");
		return 1;
	}
	lseek(fd, 1*sizeof(STUDENT), SEEK_SET);
	memset(&s, 0, sizeof(STUDENT));
	write(fd, &s, sizeof(STUDENT))
	/*
	read(fd, &s, sizeof(STUDENT));
	printf("name:%s,birth:%s,mobile:%s\n", s.name, s.birth, s.mobile);
	strcpy(s.birth, "1999-0");
	lseek(fd, -sizeof(STUDENT), SEEK_CUR);
	write(fd, &s, sizeof(STUDENT));
	close(fd);
	*/
}
