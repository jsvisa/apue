#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

//���ԣ�������̷���SIGINT���۲������Ӧ���
//Ȼ������̷���SIGQUIT���۲������Ӧ���
//������̷���SIGINT���۲������Ӧ���
int flag = 1;

void int_handler( int signum )
{
	printf("int handler %d\n",signum);
}

int count = 0;
//SIGQUIT����Ϣ������
void quit_handler( int signum )
{
	flag = 0;
	count++;
	printf("count=%d before sleep\n", count);
	sleep(10);
	printf("count=%d\n", count);
}

int main(int argc, char **argv)
{
	char choice;
	sigset_t mask;

	//ע��SIGINT����Ϣ������Ϊint_handler
	if ( signal(SIGINT, int_handler) == SIG_ERR ){
		printf("sigaction failed!\n");
		return -1;
	}

	//ע��SIGQUIT����Ϣ������Ϊquit_handler
	if ( signal(SIGQUIT, quit_handler) == SIG_ERR ){
		printf("sigaction failed!\n");
		return -1;
	}

	//��SIGINT�źżӵ������źż�����
	sigemptyset(&mask);
	if ( -1 == sigaddset(&mask,SIGINT)){
		printf("add SIGINT to mask failed!\n");
		return -1;
	}
	if ( -1 == sigprocmask(SIG_BLOCK,&mask,NULL) ){
		printf("sigprocmask failed!\n");
		return -1;
	}
	printf("SIGINT was masked, SIGQUIT was registered!\n");

	//�ȴ��ź�������
	while(flag){
		pause();
	}

	//ȡ���źŵ����� ��������źŷ�������ᱻ����
	if ( -1 == sigprocmask(SIG_UNBLOCK,&mask,NULL) ){
		printf("sigprocmask failed!\n");
		return -1;
	}

	printf("SIGINT was unmasked, pauseing .........\n");
	pause();

	return 0;
}
