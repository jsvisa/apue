#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int i;
	FILE *fp;
	pid_t pid;

	fp = fopen("log.txt", "r+");
	if(fp == NULL) {
		perror("fopen");
		return 0;
	}
	
	//printf("afdsafdsa");
	//printf("this is main");
	fwrite("I am parrent abcdefg\n", strlen("I am parrent abcdefg\n"), 1, fp);

	pid = fork();
	if(pid == 0) {
		fwrite("I am child\n", 11, 1, fp);
		fclose(fp);
		exit(0);
	}

	fwrite("bye\n", 4, 1, fp);
	sleep(1);
	fclose(fp);
	exit(0);
}
