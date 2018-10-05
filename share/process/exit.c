#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void routine1(void)//退出处理函数
{
	printf("routine1 is called.\n");
}

void routine2(void)
{
	printf("routine2 is called.\n");
}

int main(int argc, char const *argv[])
{
	atexit(routine1);//注册退出处理函数
	atexit(routine2);

	fprintf(stdout, "abcdef");//将数据输送至标准IO缓冲区

#ifdef _EXIT
	_exit(0);//直接退出
#else
	exit(0);//冲洗缓冲区数据，并执行退出处理函数
#endif
}