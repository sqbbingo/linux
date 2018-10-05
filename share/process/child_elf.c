#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("[%d]:yep,I am the child\n",(int)getpid());

#ifdef ABORT
	abort();//自己给自己发送一个致命信号AIGABRT，自杀
#else
	exit(7);//正常退出，且退出值为7
#endif
}