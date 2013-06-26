#include <sys/types.h>  /* for socket(2) and related bits and pieces */
#include <sys/socket.h> /* for socket(2) */
#include <net/if.h>     /* for struct ifreq */
#include <net/if_arp.h> /* for ARPHRD_ETHER */
#include <sys/ioctl.h>  /* for IOCTL's */
#include <stdio.h>      /* for fprintf etc */
#include <unistd.h>     /* for close */
#include <stdlib.h>
#include <setjmp.h>

static void f1(int, int, int, int, int*);
static void f2(void);

static jmp_buf jmpbuffer;
static int     globval;

int main(void)
{
	int            autoval;
	register int   regival;
	volatile int   volaval;
	static   int   statval;
	int            autoval2;

	globval =1; autoval = 2; regival = 3; volaval = 4; statval = 5;
	autoval2 = 0;

	if (setjmp(jmpbuffer) != 0)
	{
		printf("after longjmp:\n");
		printf("globval = %d, autoval = %d, regival = %d,"
				" volaval = %d, statval = %d, autoval2 = %d\n",
				globval, autoval, regival, volaval, statval, autoval2);
		//如果注释掉下面这行会有什么结果？
	//	exit(0);
	}

	globval =95; autoval = 96; regival = 97; volaval = 98; statval = 99;

	f1( autoval, regival, volaval, statval, &autoval2);
	exit(0);
}

static void f1(int i, int j, int k, int l, int* m)
{
	printf("in f1():\n");
	printf("globval = %d, autoval = %d, regival = %d,"
			" volaval = %d, statval = %d, autoval2 = %d\n",
			globval, i, j, k, l, *m);
	*m = 11;
	f2();
}

static void f2(void)
{
	longjmp(jmpbuffer, 1);
}


