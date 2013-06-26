#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int i;
	int fd;
	int spfd;
	
	char *srcFile;
	char spFile[128];
	char nextFile[128];
	char buf[4096];
	int left ;
	int size;

	if(argc != 3) {
		printf("usage: compose sp_filename dest_file\n");
		return 0;
	}

	srcFile = argv[2];
	strcpy(spFile, argv[1]);
	printf("first file:%s, dest_file:%s\n", spFile, srcFile);

	fd = open(srcFile, O_CREAT|O_RDWR|O_TRUNC, S_IRWXU);
	if(fd < 0) {
		perror("Open");
		return 0;
	}

	while(1) {
		spfd = open(spFile, O_RDONLY);
		if(spfd < 0) {
			perror("open");
			break;
		}
		read(spfd, buf, 8);
		if(strncmp(buf, "splitV10", 8) != 0) {
			printf("Invalid file head\n");
			close(spfd);
			break;
		}
		if(read(spfd, spFile, 128) != 128) {
			printf("Read file name failed\n");
			break;
		}
		if(read(spfd, &left, 4) != 4) {
			printf("Read length failed\n");
			break;
		}
		if(read(spfd, spFile, 128) != 128) {
			printf("Read next filename failed\n");
			break;
		}

		lseek(spfd, 4, SEEK_CUR);

		while(left > 0) {
			size = read(spfd, buf, left<4096?left:4096);
			if(size < 0) {
				printf("File is error\n");
				break;
			}
			write(fd, buf, size);
			left -= size;
		}
		close(spfd);
		if(spFile[0] == '\0')
			break;
	}
	printf("OK!\n");
	close(fd);
	return 0;
}
