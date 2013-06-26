#include <dirent.h>
#include <linux/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//判断是不是一个目录
int isDir(const char *fname) {
	struct stat statbuf;
	memset(&statbuf, 0, sizeof(struct stat));
	if (stat(fname, &statbuf) < 0)
	        return 0;
	if(S_ISDIR(statbuf.st_mode))
		return 1;
	return 0;
}
int accessDir(const char *path) {
	DIR *dirp;
	struct dirent *dir;
	char fname[1024];
	if((dirp = opendir(path)) == NULL) {
		perror("Open dir failed");
		return -1;
	}

	while((dir = readdir(dirp))) {
		if(strcmp(dir->d_name, ".") == 0||
				strcmp(dir->d_name, "..") == 0) {
			continue;
		}

		sprintf(fname, "%s/%s", path, dir->d_name);
		if(isDir(fname)) {
			printf("dirctory %s\n", dir->d_name);
			accessDir(fname);
		}
		else
			printf("file %s\n", dir->d_name);
	}
	closedir(dirp);
	return 1;
}

int main(int argc, char **argv) {
	if(argc < 2) {
		accessDir("./");
	}
	else
		accessDir(argv[1]);
	return 0;
}
