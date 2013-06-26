#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
extern char **environ;

int main(int argc, char **argv) {
	int i=0; 
	char *pwd;
	pwd = getenv("PWD");
	printf("PWD=%s\n", pwd);
	
	setenv("DBSYS", "/tmp/student.db", 1);
	getchar();

	for(i=0; environ[i]!=NULL; i++){
			printf("%s\n",environ[i]);
	}
	return 0;
}
