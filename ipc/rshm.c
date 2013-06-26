#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
	int shm_id,i;
	key_t key;
	char* name = "/dev/zero";
	struct shmid_ds ds;
	char *shm;

	key = ftok(name,0);
	if(key == -1){
		printf("ftok error\n");
	}

	shm_id = shmget(key,4096,IPC_CREAT);	
	printf("id=%d\n", shm_id);
	if(shm_id == -1){
		printf("shmget error\n");
		return -1;
	}

	shm = shmat(shm_id,NULL,0);

	printf("Content is:%s\n",shm);

	/*
	shmctl(shm_id, IPC_STAT, &ds);
	if(shmctl(shm_id, SHM_LOCK, &ds) != 0) {
		perror("shmctl");
	}
	*/
	strcpy(shm, argv[1]);

	if(shmdt(shm) == -1){
		printf(" detach error\n");
	}
	return 0;
}
