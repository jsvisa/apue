#include <setjmp.h>
#include <stdio.h>

jmp_buf env;
void do_something(int j) {
	printf("I am in do something\n");
	printf("before long jump\n");

	if(j == 0)
		longjmp(env, 9);
	else
		printf("Don't jump\n");

}

void test_jump(int j) {
	printf("I am in test jump\n");
	do_something(j);
	printf("quit test_jump\n");
}
int main(int argc, char **argv)
{
	int ret;
	int auto_val=9;
	volatile vol_val=10;

	if( 0 == (ret=setjmp(env)) ){
		printf("set jump succeed! ret=%d auto_val=%d, vol_val=%d\n",ret,auto_val,vol_val);
		auto_val++;
		vol_val++;
		test_jump(0);
		printf("never go here\n");
		return 0;
	}    
	/*
	else if(ret == 9) {
		test_jump(0);
	}
	*/

	printf("after long jump!ret=%d,auto_val=%d, vol_val=%d\n",ret,auto_val,vol_val);

	return -1;
}

