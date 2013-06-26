#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

 

//�����ĵ�һ���߳���ע����źŴ�����
void sighandler(int sig) { 
        printf("SIG:I am thread:%u\n", pthread_self()); 
} 


//�����ĵڶ����߳�ע����źŴ�����
void sighandler2(int sig) { 
        printf("SIG2:I am thread:%u\n", pthread_self()); 
} 


//��������ע����źŴ�����
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


       //ע���������У�ִ�лᷢ���������ڽ��յ��ź�ʱ��Ȼ��������߳�ע����źŴ�����
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
        // 1. ע���������д��룬Ȼ����˽��̷����źţ�
		//    �ᷢ���ź�ʼ���Ǳ������̴����

        // 2. ע���������д��룬ȡ�������ź����δ���ε�ע�ͣ�
		// ִ�в���˽��̷����źţ��ᷢ���źű����������߳���
		// ������һ������
        
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
