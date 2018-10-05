#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	pid_t x;
	x = fork();

	if (x > 0)//父进程
	{
		printf("[%d]:I am the child\n",(int)getpid());
		exit(0);
	}

	if (x == 0)//子进程
	{
		printf("[%d]:I am the child\n",(int)getpid() );
		execl("./child_elf","child_elf",NULL);

		printf("never be prited\n");//这是一条被覆盖的代码
	}
	
	return 0;
}