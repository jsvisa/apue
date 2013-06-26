#include <dirent.h>
#include <errno.h>
#include <stdio.h>
int main(int argc, char **argv) {
	DIR *dir;
	struct dirent *dirt;

	dir = opendir(argv[1]);
	if(dir == NULL) {
		perror("opendir failed");
		return 0;
	}
	else
		printf("Open success\n");

	while((dirt = readdir(dir)) != NULL) {
		printf("d_name=%s\n", dirt->d_name);
	}
	closedir(dir);
	return 0;
}
