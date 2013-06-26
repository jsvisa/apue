#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int c;
void sum(int n) {
	int i;
	int count = 0;
	for(i=1; i<=n; i++) {
		count += i;
	}
	printf("Count = %d\n", count);
}
int main( int argc, char **argv)
{
	int i,j;
	int a=10;
	char *p;
	p = (char *)malloc(10);
	pid_t pid, pid1;
	printf("test fork\n");

	int fd;
	char buf[100];
	int pt;
	int status;
	memset(buf, 0, sizeof(buf));
	printf("AAAAAAAAAAAAAAAAAAAAAB");
	for(j=0; j<100; j++) {
		pid = fork();
		if(pid > 0) {
			/*
			   pid = fork();
			   if(pid > 0) {
			   pt = wait(&status);
			   printf("Child %d quit\n", pt);
			   }
			   else if(pid == 0) {
			   sleep(100);
			   }
			   */
		}	
		else if(pid == 0) {
			printf("child %d\n", getpid());
			for(i=0; i<10; i++) {
				printf("child said:%d\n", i);
				sleep(2);
			}
			exit(0);
		}
	}
	printf("process %d quit\n", getpid());
	/*
	   printf("before fork pid=%d\n", getpid());

	   int fd;
	   fd = open("test.txt", O_CREAT|O_RDWR, S_IRWXU);
	   if(fd < 0) {
	   printf("Open failed\n");
	   return 0;
	   }
	   */
	/*
	   if(pid > 0){ //父进程
	   int size;
	   char buf[20];
	   sleep(1);
	   a = 15;
	   printf("This is parent process! my pid=%d, child pid=%d, a=%d, &a=%x\n", 
	   getpid(), pid, a, &a);
	   size = read(fd, buf, 10);
	   printf("Read %d bytes:%s\n", size, buf);
	   }
	   else if ( pid == 0 ){ //进入子进程
	   a++;
	   printf("This is child process! parent pid=%d,my pid=%d,  "
	   "pid=%d, a=%d, &a=0x%x\n",getppid(),getpid(),pid,a, &a);
	   write(fd, "child", 5);
	   lseek(fd, 2, SEEK_SET);
	   close(fd);
	   printf("file closed by child\n");
	   }
	   else if( pid<0){
	   printf("fork failed!\n");
	   return -1;
	   }
	   */

	/* 僵尸进程产生
	   if(pid > 0) {
	   printf("Process %d quit.\n", (int)getpid());
	   sleep(20);
	   getchar();
	   exit(0);
	   }
	   else if(pid ==0)  {
	   printf("This is child:%d\n", (int)getpid());
	   printf("Child process quit now.\n");
	   exit(0);
	   }
	   else {
	   printf("fork failed\n");
	   }
	   */

	/*
	   int i = 5;
	   printf("hello");
	   for(i=0; i<10; i++) {
	   pid = fork();
	   if(pid == 0) {
	   printf("Child %d\n", getpid());
	   exit(0);
	   }
	   }
	   sleep(50);
	   */
	return 0;
}
