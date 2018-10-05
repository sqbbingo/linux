#include <stdio.h>
#include <unistd.h>

int main(void)
{
	printf("[%d]:before fork()...\n",(int)getpid());

	pid_t x;
	x = fork();//生个孩子

	printf("[%d]:after fork()...\n",(int
		)getpid());
	return 0;
}