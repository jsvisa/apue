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


/*
 * sembuf{
 *   short int sem_num; //semaphore number
 *   short int sem_op; //semaphore operaion
 *   short int sem_flg //operation flag
 * };
 */

//生成信号量
int sem_creat(key_t key, int num)
{
	union semun sem;
	int semid;
	sem.val = num;
	semid = semget(key, 1, IPC_CREAT|0666);
	if (-1 == semid){
		printf("create semaphore error\n");
		exit(-1);
	}
	//初始值设成3
	semctl(semid, 0, SETVAL, sem);
	return semid;
}
//删除信号量
void del_sem(int semid)
{
	union semun sem;
	sem.val = 0;
	semctl(semid, 0, IPC_RMID, sem);
}

//减操作
int p(int semid) {
	struct sembuf sops={0,-1,SEM_UNDO};
	semop(semid,&sops,1);
}

//加操作
int v(int semid) {
	struct sembuf sops={0,+1,0};
	semop(semid,&sops,1);
}

int main(int argc, char **argv) {
	key_t key;
	int shmid,semid;
	char *shm;
	char msg[7] = "-data-";
	char i;
	struct semid_ds buf;
//	union semun sem;
	unsigned short val[5];
	int ss;

	key = ftok("/",0);
	
	shmid = shmget(key, SEGSIZE, IPC_CREAT|0604);
	if (-1 == shmid){
		printf(" create shared memory error\n");
		return -1;
	}
	shm = (char *)shmat(shmid,0,0);
	if (-1 == (int)shm){
		printf(" attach shared memory error\n");
		return -1;
	}
	strcpy(shm, "first");

	//创建一个信号量，最多允许3个人同时占有该资源
	semid = sem_creat(key, 3);
	semctl(semid, 1, GETALL, val);
	ss = semctl(semid, 0, GETVAL);
	printf("ss= %d\n", ss);
	
	/*
	for (i = 0;i <= 3;i++){
		//减，占有该资源
		ss = p(semid);
		msg[5] = '0' + i;
//		semctl(semid, 0, IPC_STAT, buf);
		ss = semctl(semid, 0, GETVAL);
		shmctl(semid, 0, GETALL, val);
		memcpy(shm,msg,sizeof(msg));
		sleep(5);
		//加，释放该资源
		ss = v(semid);
		ss = semctl(semid, 0, GETVAL);
		printf("v op ok\n");
	}
	*/
	sleep(300);
	shmdt(shm);
	shmctl(shmid, IPC_RMID, &buf);
	del_sem(semid);
	return 0;
}

