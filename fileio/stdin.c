#include <unistd.h>

int main(int argc, char **argv) {
	close(1);
	printf("1 is closed\n");
	write(2, "1 is closed\n", 12);
	return 0;
}
