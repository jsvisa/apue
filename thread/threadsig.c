#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

 

//创建的第一个线程中注册此信号处理函数
void sighandler(int sig) { 
        printf("SIG:I am thread:%u\n", pthread_self()); 
} 


//创建的第二个线程注册此信号处理函数
void sighandler2(int sig) { 
        printf("SIG2:I am thread:%u\n", pthread_self()); 
} 


//主进程中注册此信号处理函数
void sighandler_m(int sig) { 
        printf("SIG:I am main process:%u.\n", pthread_self()); 
} 

void *mThread(void *args) { 
        int i = *(int*)args; 
        pthread_detach(pthread_self()); 
		
        printf("I am thread %d:%u\n", i, pthread_self()); 
        if(i == 1) {
                signal(SIGUSR1, sighandler); 
		}
        else if(i == 2) 
                signal(SIGUSR1, sighandler2); 

		while(1) {
	        sleep(100); 
			printf("Thread %d  %u wakeup\n", i, pthread_self()); 
		}
        return NULL; 
} 

int main(int argc, char **argv) { 
        pthread_t tid1, tid2; 
        sigset_t mask; 
        int i; 

        printf("PID:%d\n", getpid()); 

        printf("I am main process:%u.\n", pthread_self()); 


       //注释下面这行，执行会发现主进程在接收到信号时仍然会进入子线程注册的信号处理函数
		signal(SIGUSR1, sighandler_m); 
        i=1; 
        pthread_create(&tid1, NULL, mThread, &i); 
        sleep(1); 
        i=2; 
        pthread_create(&tid2, NULL, mThread, &i); 

//		getchar();
		/*
        sleep(1); 
        pthread_kill(tid1, SIGUSR1); 
        sleep(1); 
        pthread_kill(tid2, SIGUSR1); 
		*/
        // 1. 注释上面四行代码，然后向此进程发送信号，
		//    会发现信号始终是被主进程处理的

        // 2. 注释上面四行代码，取消下面信号屏蔽代码段的注释，
		// 执行并向此进程发送信号，会发现信号被创建的子线程中
		// 的其中一个处理
        
        sigemptyset(&mask); 
        if ( -1 == sigaddset(&mask,SIGUSR1)){ 
                printf("add SIGINT to mask failed!\n"); 
                return -1; 
        } 
        if ( -1 == sigprocmask(SIG_BLOCK, &mask, NULL) ){ 
                printf("sigprocmask failed!\n"); 
                return -1; 
        } 
 
        sleep(100); 
        printf("Main process wake\n"); 
        sleep(3); 
        return 0; 
}
