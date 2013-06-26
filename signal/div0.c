#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int i;
void int_handler(int num) {
	int size;
	char buf[101];
	i++;
	printf("sleep start %d\n", i);
	sleep(30);
	printf("Please input:\n");
	size = read(STDIN_FILENO, buf, 100);
	if(size > 0) {
		buf[size] = 0;
		printf("Read %d:%s\n", size, buf);
	}
	else
		printf("Read failed\n");
	printf("sleep end %d\n", i);
}
void term_handler(int num) {
	printf("receive term signal\n");
}

void testloop() {
	while(1) {
		printf("Start to sleep\n");
		sleep(5);
		printf("Sleep end\n");
	}
}
int main(int argc, char **argv) {
	int a;
	a = 100/0;
	printf("quit\n");
	/*
	struct sigaction act, oact;
	sigemptyset(&act.sa_mask);
    act.sa_flags |= SA_RESTART;
	act.sa_handler = int_handler;
	if(sigaction(SIGINT, &act, &oact) < 0) {
		printf("regist handler failed\n");
		return 0;
	}
	act.sa_handler = term_handler;
	if(sigaction(SIGTERM, &act, &oact) < 0) {
		printf("regist handler failed\n");
		return 0;
	}
	testloop();
	*/
//	printf("Set signal default\n");
//	signal(SIGINT, SIG_DFL);
//	getchar();
	return 0;
}
