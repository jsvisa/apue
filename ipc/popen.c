#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

int main(int argc, char **argv) {
	char line[4096];
	FILE *fin;
	int i=0;

	fin = popen("./simp", "w");
	if (NULL == fin){
		printf("error popen:%s\n", strerror(errno));
		return -1;
	}

	fwrite("popen test", 10, 1, fin);
	/*
	while(true){
		if (NULL == fgets(line, 4096,fin) ){
			break;
		}

		printf("%d %s",++i,line);
	}
	*/
	pclose(fin);

	return 0;
}
