#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>
#define PERM_FILE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/*[pgm]*/
int shmid;
static int *getaddr(void)
{
	key_t key;
	int *p;
	int fd;
	/*
	fd = open("shmseg", O_WRONLY | O_CREAT, 0);
	if(fd < 0) {
	}
	close(fd);
	*/
	key = ftok("shmseg", 1);
	shmid = shmget(key, sizeof(int), IPC_CREAT | PERM_FILE);
	printf("shmid=%d\n", shmid);
	p = shmat(shmid, NULL, 0);

	printf("shared memory attached from %x\n", p);
	return p;
}

int main(void)
{
	pid_t pid;

	if ((pid = fork()) == 0) {//вс╫ЬЁл
		int *p, prev = 0;

		p = getaddr();
		while (*p != 99) {
			if (prev != *p) {
				printf("child saw %d\n", *p);
				prev = *p;
			}
		}
		printf("child is done\n");
		shmdt(p);
	}
	else {
		int *p;

		p = getaddr();
		for (*p = 1; *p < 4; (*p)++)
			sleep(1);

		*p = 99;
		sleep(1);
		shmdt(p);
	}
	shmctl(shmid, IPC_RMID, 0);
	exit(EXIT_SUCCESS);
}

