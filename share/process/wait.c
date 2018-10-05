#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	pid_t x = fork();

	if (x == 0)//子进程，执行指定程序child_elf
	{
		execl("./child_elf","child_elf",NULL);
	}

	if (x > 0)//父进程，使用wait()阻塞等待子进程的退出
	{
		int status;
		wait(&status);

		if (WIFEXITED(status))//判断子进程是否正常退出
		{
			printf("child exit normally,"
				"exit value:%hhu\n",WEXITSTATUS(status));
		}

		if (WIFSIGNALED(status))//判断子进程是否被信号杀死
		{
			printf("child killed by signal:%u\n",
				WTERMSIG(status) );
		}
	}
	
	return 0;
}