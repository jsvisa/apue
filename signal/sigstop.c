#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int global = 0;

void handler2(int num) {
	printf("I am handler 2\n");
}

void intHandler(int num) {
	if(num == SIGINT) {
		printf("Received SIGINT\n");
	}
	else if(num == SIGTERM) {
		printf("Received SIGTERM\n");
	}

	sleep(9);
	printf("Wake up now\n");
	/*
	if(global == 3) {
		signal(SIGINT, SIG_DFL);
	}
	else {
		printf("I received signal %d\n", num);
		sleep(3);
		printf("I am wakeup\n");
		global++;
	}
	*/
}

int main(int argc, char **argv) {
	int i;

	signal(SIGINT, intHandler);
	signal(SIGTERM, intHandler);

	for(i=0;;i++) {
		printf("this is %d\n", i);
		if(global >= 3)
			break;
		sleep(1);
	}
	signal(SIGINT, handler2);
	for(i=0;;i++) {
		printf("this is %d\n", i);
		sleep(1);
	}
	return 0;
}
