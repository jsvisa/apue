#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>



int main(int argc, char **argv)
{
	printf("term int pause\n");
	while(1) {
		pause();
		printf("child wake...");
	}
	printf("term int wakeup\n");

    return 0;
}
