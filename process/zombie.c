#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
	pid_t pid;
	int fd;
	char line[1024];
	FILE *fp;
	int status;

	pid = fork();
	if(pid == 0) {
		exit(0);
	}
	system("ps -ef > a.txt");
	fp = fopen("a.txt", "r");
	if(fp == NULL) {
		perror("fopen");
		return 1;
	}
	while(fgets(line, sizeof(line), fp) != NULL) {
		if(strstr(line, "[a.out] <defunct>") != NULL) {
			printf("Find zombie process:%s\n", line);
			break;
		}
	}
	fclose(fp);

	wait(&status);

	system("ps -ef > a.txt");
	fp = fopen("a.txt", "r");
	if(fp == NULL) {
		perror("fopen");
		return 1;
	}
	while(fgets(line, sizeof(line), fp) != NULL) {
		if(strstr(line, "[a.out] <defunct>") != NULL) {
			printf("Find zombie process:%s\n", line);
			break;
		}
	}
	fclose(fp);

	return 0;
}
