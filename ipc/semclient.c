#include <sys/sem.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>

#define SEGSIZE 1024
#define READTIME 1
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} arg;

// 打印程序执行时间
void out_time(void)
{
	static long start = 0;
	time_t tm;
	if (0 == start){
		tm = time(NULL);
		start = (long)tm;
		printf(" now start ...\n");
	}
	printf(" second: %ld \n",(long)(time(NULL)) - start);
}

//创建信号量
int new_sem(key_t key)
{
	union semun sem;
	int semid;
	sem.val = 0;
	semid = semget(key, 0, 0);
	if (-1 == semid){
		printf("create semaphore error\n");
		exit(-1);
	}
	return semid;
}

//等待信号量变成0
void wait_v(int semid)
{
	struct sembuf sops={0,0,0};
	printf("semop start\n");
	semop(semid,&sops,1);
}

//减操作
int p(int semid, int num) {
	struct sembuf sops={0,-num,0};
	semop(semid, &sops,1);
}

//加操作
int v(int semid, int num) {
	struct sembuf sops={0,+num,0};
	if(semop(semid, &sops, 1) != 0) {
		perror("semop");
		return -1;
	}
	return 0;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Arguments error.\n");
		exit(-1);
	}
	key_t key;
	int shmid,semid;
	char *shm;
	char msg[100];
	int i;
	int num;

	key = ftok("/", 0);
	shmid = shmget(key, SEGSIZE, 0);

	if(-1 == shmid){
		printf(" create shared memory error\n");
		return -1;
	}
	shm = (char *)shmat(shmid,0,0);
	if (-1 == (int)shm){
		printf(" attach shared memory error\n");
		return -1;
	}
	semid = new_sem(key);
	//申请进入房间
	printf("正在准备进入房间...\n");
	p(semid, 1);
	printf("已进入!\n");
	printf("shm=%s\n", shm);
	sleep(3);
	strcpy(shm, argv[1]);
	num = semctl(semid, 0, GETVAL);
	printf("当前房间还允许%d人进入\n", num);
	//TODO 在此处添加代码，做自己要做的事情
	//.......
	//事情做完，退出房间
	printf("请按回车退出房间\n");
	getchar();
	v(semid, 1);
	/*
	for(i=1; i<100000; i++) {
		if(v(semid, i) != 0) {
			num = semctl(semid, 0, GETVAL);
			printf("num=%d\n", num);
			break;
		}
	}
	*/
	shmdt(shm);
	return 0;
	// gcc -o shmc shmC.c -g
}

