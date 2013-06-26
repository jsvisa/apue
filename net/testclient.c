#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <stdlib.h>
#include "csocket.h"

#define PTHREAD_STACK_MIN 8

char host[32];
int port;
void *TestThread(void *arg) {
	int num;
	int sock;
	num = *(int *)arg;
	char buf[100];
	int size;

	free(arg);
	sleep(500);
	printf("Start thread:%d\n", num);
	while(1) {
		sock = myConnect(host , port);
		if(sock < 0) {
			perror("connect");
			continue;
		}
		sprintf(buf, "[%u]I am thread %d\n", (unsigned int)pthread_self(), num);
		while(1) {
			size = send(sock, buf, strlen(buf), 0); 
			if(size <= 0) {
				perror("SEND");
				break;
			}

			size = recv(sock, buf, sizeof(buf), 0);
			if(size <= 0) {
				perror("recv");
				break;
			}
			buf[size] = 0;
			printf("%s");
			sleep(1);
		}
		close(sock);
	}
	return NULL;
}

void handler(int sig) {
}

int main(int argc, char **argv) {
	int i;
	int num;
	int *p;
	pthread_t threadID;

	num = atoi(argv[1]);

	pthread_attr_t attr;
	size_t stackSize;

	//设置允许打开的文件最大数
	struct rlimit rt;
	rt.rlim_max = rt.rlim_cur = 5000;
	if (setrlimit(RLIMIT_NOFILE, &rt) == -1) {
		perror("setrlimit");
		return 1;
	}

	//设置线程栈空间大小
	pthread_attr_init(&attr);
	stackSize = PTHREAD_STACK_MIN * 8;
	pthread_attr_setstacksize(&attr, stackSize);

	strcpy(host, argv[2]);
	port = atoi(argv[3]);
	for(i=0; i<num; i++) {
		p = malloc(sizeof(int));
		*p = i;
		if(pthread_create(&threadID, &attr, TestThread, p) != 0) {
			perror("pthread_create");
			sleep(1);
		}
	}
	getchar();
}
