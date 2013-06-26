#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>


static pthread_mutex_t *mptr;

int main(int argc, char **argv) {
	int fd;
	pid_t pid;

	key_t key;
	int shmid;
	char *shm;
	pthread_mutexattr_t mattr;

	/*
	key = ftok("/",0);
	shmid = shmget(key, sizeof(pthread_mutex_t), IPC_CREAT|0604);
	if (-1 == shmid){
		printf(" create shared memory error\n");
		return -1;
	}
	mptr = (pthread_mutex_t *)shmat(shmid,0,0);
	*/

	//mptr = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	
	fd = open("/dev/zero", O_RDWR, 0);
	mptr = (pthread_mutex_t *)mmap(0, sizeof(pthread_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	
//	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
//	pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_PRIVATE);
	pthread_mutex_init(mptr, &mattr);
	
	//pthread_mutex_init(mptr, NULL);

	pid = fork();
	if(pid == 0) {
		if(pthread_mutex_lock(mptr) == 0) {
			printf("Process %d lock success\n", getpid());
			sleep(10);
			pthread_mutex_unlock(mptr);
		}
		else 
			perror("lock");
		exit(0);
	}

	if(pthread_mutex_lock(mptr) == 0) {
		printf("Process %d lock success\n", getpid());
		sleep(10);
		pthread_mutex_unlock(mptr);
	}
	else 
		perror("lock");
	pthread_mutex_destroy(mptr);
	munmap(mptr, sizeof(pthread_mutex_t));
	return 0;
}
