#include <stdio.h>
#include <errno.h>

int main(int argc, char **argv) {
	int i;
	FILE *fp;
	fp = fopen("./b.txt", "w+");
	if(fp == NULL) {
		perror("Open failed");
		return 1;
	}
//	for(i=0; i<10000; i++) {
		fwrite("12345678", 8, 1, fp);
//		usleep(10000);
//	}
	printf("write over\n");
	getchar();
	fclose(fp);	
	return 0;
}
