#include <time.h>

struct timeval {
	unsigned long tv_sec;
	unsigned short tv_usec;
};

int main(int argc, char **argv) {
	time_t t;
	struct timeval tv;
	time(&t);
	printf("time:%d\n", t);
	gettimeofday(&tv);
	printf("%d.%d\n", tv.tv_sec, tv.tv_usec);
	return 0;
}
