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
	int splitSize;
	int remainSize; //剩余的字节数
	
	char *srcFile;
	char spFile[128];
	int num; //文件分成的块数
	char buf[4096];
	int count;
	int fileSize;
	struct stat st;
	int left;
	int size;

	if(argc != 3) {
		printf("usage: split filename split_size\n");
		return 0;
	}
	srcFile = argv[1];
	splitSize = atoi(argv[2]);
	if(splitSize <= 0) {
		splitSize = 1024*1024;
	}
	fd = open(srcFile, O_RDONLY);
	if(fd < 0) {
		perror("Open");
		return 0;
	}
	fstat(fd, &st);
	num = st.st_size/splitSize;
	if((remainSize = st.st_size % splitSize) != 0)
		num++;

	for(i=0; i< num ;i++) {
		sprintf(spFile, "%s_%03d", srcFile, i);
		spfd = open(spFile, O_CREAT|O_RDWR|O_TRUNC, S_IRWXU);
		if(spfd < 0) {
			perror("open");
			break;
		}
		write(spfd, "splitV10", 8);
		write(spfd, spFile, sizeof(spFile));
		if(i<num-1) {
			write(spfd, &splitSize, 4);
			sprintf(spFile, "%s_%03d", srcFile, i+1);
		}
		else {
			memset(spFile, 0, sizeof(spFile));
			write(spfd, &remainSize, 4);
		}
		write(spfd, spFile, sizeof(spFile));
		write(spfd, &splitSize, 4);
		left = (i<num-1)?splitSize:remainSize;
		count = 0;
		while(left > 0) {
			size = read(fd, buf, left<4096?left:4096);
			if(size < 0) {
				break;
			}
			write(spfd, buf, size);
			left -= size;
		}
		close(spfd);
	}
	close(fd);
	return 0;
}
