#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>
#define PERM_FILE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


#define ARRAY_SIZE   40000
#define MALLOC_SIZE 10000
#define SHM_SIZE    1000
#define SHM_MODE    (SHM_R | SHM_W)

char array[ARRAY_SIZE];

int main(int argc, char **argv) {
	int shmid;
	key_t key;
	char *ptr, *shmptr;

	printf("array[] from %x to %x\n", &array[0], &array[ARRAY_SIZE]);
	printf("stack around %x\n", &shmid);
	if((ptr = malloc(MALLOC_SIZE)) == NULL) {
		printf("Malock failed:%s\n", strerror(errno));
		exit(0);
	}
	printf("malloced from %x to %x\n", ptr, ptr+MALLOC_SIZE);

	close(open("shmseeg", O_WRONLY | O_CREAT, PERM_FILE));
	key = ftok("shmseeg", 1);

	if((shmid = shmget(key, SHM_SIZE, IPC_CREAT | PERM_FILE)) < 0) {
		printf("shmget error:%s\n", strerror(errno));
		exit(0);
	}
	if((shmptr = shmat(shmid, NULL, 0)) == (void *)-1) {
		printf("shmat error:%s\n", strerror(errno));
		exit(0);
	}
	printf("shared memory attached from %x to %x\n", shmptr, shmptr+SHM_SIZE);
	shmctl(shmid, IPC_RMID, 0);
	exit(0);		
}
