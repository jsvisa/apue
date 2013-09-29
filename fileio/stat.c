#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	struct stat st;
	int ret;
	if(argc < 2) {
		printf("Command ./stat filename\n");
		return 0;
	}

	if(access(argv[1], F_OK) != 0) {
		printf("File not exists\n");
		return 0;
	}

	ret = stat(argv[1], &st);
	if(ret != 0) {
		perror("stat failed");
		return 0;
	}
	printf("file size:%d,uid:%d\n", (int)st.st_size, (int)st.st_uid);
	return 0;
}
