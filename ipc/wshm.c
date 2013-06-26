#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

//·þÎñÆ÷
int main(int argc, char** argv)
{
	int shm_id,i;
	key_t key;
	char* name = "/dev/zero";
	struct shmid_ds ds;
	char *shm;

	key = ftok(name, 0);
	if(key==-1){
		printf ("ftok error %s \n",strerror(errno));
		return -1;
	}

	shm_id=shmget(key,4096,IPC_CREAT|0600);	
	printf("id=%d\n", shm_id);
	if(shm_id==-1) {
		printf("shmget error %s\n",strerror(errno));
		return -1;
	}

	shm = shmat(shm_id, NULL, 0);
	if(-1 == *(int *)shm){
	   printf("shmat failed %s\n", strerror(errno));
	   return -1;
	}

	strncpy(shm,"hello shm....",13);

	/*
	shmctl(shm_id, IPC_STAT, &ds);
	if(shmctl(shm_id, SHM_LOCK, &ds) != 0) {
		perror("shmctl");
	}
	printf("Lock succes\n");
	getchar();

	if(shmctl(shm_id, SHM_UNLOCK, &ds) != 0) {
		perror("shmctl");
	}
	*/
	printf("shm=%s\n", shm);
	if(shmdt(shm)==-1){
		printf("shmdt error %s\n",strerror(errno));
		return -1;
    }
	return 0;
}
