#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "csocket.h"

#define MAX_THREAD 500
#define INIT_NUM 5

int curThreadNum = 0;

class MThread {
public:
	pthread_t tid;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	bool isIdle;

public:
	MThread() {
		isIdle = false;
		pthread_mutex_init(&mutex, NULL);
		pthread_cond_init(&cond, NULL);
		pthread_create(&tid, NULL, ThreadFun, this);
	}
	virtual ~MThread() {
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cond);
	}
	void run() {
		while(1) {
			pause();
			start();
		}
	}
	bool pause() {
		printf("pause now...\n");
		if(pthread_mutex_lock(&mutex) != 0)
			return false;
		isIdle = true;
		pthread_cond_wait(&cond, &mutex);
		printf("Thread %u is wakeup\n", (unsigned int)tid);
		pthread_mutex_unlock(&mutex);
		
		return true;
	}
	bool isBusy() {
		return !isIdle;
	}
	bool wakeup() {
		printf("Try to wake up thread\n");
		if(pthread_mutex_lock(&mutex) != 0)
			return false;
		isIdle = false;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		return true;
	}
public:
   	static void *ThreadFun(void *arg) {
		MThread *thread = (MThread *)arg;
		pthread_detach(pthread_self());
		printf("Create a thread\n");
		thread->run();
	}
	virtual bool start() = 0;
};

class SessionThread:public MThread {
	int socket;

public:
	void setSock(int fd) {
		socket = fd;
	}
	~SessionThread() {
	}
	bool start() {
		char req[1024];
		while(1) {
			if(getRequest(req) <= 0)
				break;
			printf("Get Req :%s\n", req);
			sleep(10);
			dealRequest(req);
			dealRequest(req);
			dealRequest(req);
		}
		close(socket);
		return true;
	}
	int getRequest(char *req) {
		int size;
		printf("Get request\n");
		size = recv(socket, req, 1024, 0);
		if(size <= 0) {
			perror("recv");
			return -1;
		}
		req[size] = 0;
		return size;
	}
	bool dealRequest(char *req) {
		int size;
		if((size = send(socket, req, strlen(req), 0)) <= 0) {
			perror("send");
		}
		printf("send size:%d\n", size);
		return true;
	}
};

int main(int argc, char **argv) {
	int i;
	int client;
	int servSock;
	socklen_t len;
	struct sockaddr sa;
	SessionThread thread[MAX_THREAD];

	getchar();
	signal(SIGPIPE, SIG_IGN);
	servSock = socket(AF_INET, SOCK_STREAM, 0);
	if(myBind(servSock, NULL, 8000) != 0) {
		perror("bind");
		close(servSock);
		return 1;
	}
	listen(servSock, 10);

	bool got = false;
	while(1) {
		len = sizeof(sa);
		got = false;
		client = accept(servSock, &sa, &len);
		if(client >= 0) {
			for(i=0; i<MAX_THREAD; i++) {
				if(!thread[i].isBusy()) {
					thread[i].setSock(client);
					thread[i].wakeup();
					got = true;
					break;
				}
			}
		}
	}
}
