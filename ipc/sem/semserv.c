#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SEGSIZE 1024
#define READTIME 1

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} arg;

//�����ź���
int sem_creat(key_t key, int num)
{
	union semun sem;
	int semid;
	sem.val = num;
	semid = semget(key, 2, IPC_CREAT|0666);
	if (-1 == semid){
		printf("create semaphore error\n");
		exit(-1);
	}
	//��ʼֵ���3
	semctl(semid, 0, SETVAL, sem);
	semctl(semid, 1, SETVAL, sem);
	return semid;
}

//ɾ���ź���
void del_sem(int semid)
{
	union semun sem;
	sem.val = 0;
	semctl(semid, 0, IPC_RMID, sem);
}

//������
int p(int semid, int num) {
	struct sembuf sops={num, -1, 0};
	semop(semid, &sops,1);
}

//�Ӳ���
int v(int semid, int num) {
	struct sembuf sops={num, +1, 0};
	if(semop(semid, &sops, 1) != 0) {
		perror("semop");
		return -1;
	}
	return 0;
}

int main(int argc, char **argv) {
	key_t key;
	int semid;
	unsigned short val[5];

	key = ftok("/",0);
	
	//����һ���ź������������3����ͬʱռ�и���Դ
	semid = sem_creat(key, 3);
	while (1)
	{
		semctl(semid, 0, GETALL, val);
		printf("%d, %d\n", val[0], val[1]);
		sleep(1);
	}
	
	del_sem(semid);
	return 0;
}

