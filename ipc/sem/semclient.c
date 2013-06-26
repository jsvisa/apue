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
	struct sembuf sops={num, -1, 0};
	semop(semid, &sops,1);
}

//加操作
int v(int semid, int num) {
	struct sembuf sops={num, +1, 0};
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
	int choose = atoi(argv[1]);

	if (choose != 0 && choose != 1)
	{
		printf("0代表男生，1代表女生...\n");
		return -1;
	}

	key_t key;
	int semid;
	int print_flg = 1;
	unsigned short val[5];

	key = ftok("/", 0);

	semid = new_sem(key);
	//申请进入房间

	printf("正在准备进入房间...\n");

	semctl(semid, 0, GETALL, val);

	while (val[choose ? 0:1] < 3)
	{
		if (print_flg == 1)
		{
			print_flg = 0;
			printf("请等待.......\n");
		}
		semctl(semid, 0, GETALL, val);
	}
	print_flg = 1;

	p(semid, choose);
	printf("已进入!\n");
	sleep(3);
	semctl(semid, 0, GETALL, val);
	printf("当前房间还允许%d人进入\n", val[choose]);
	printf("请按回车退出房间\n");
	getchar();
	v(semid, choose);
	return 0;
}

