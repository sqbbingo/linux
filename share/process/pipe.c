#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	int fd[2];//用来存放PIPE的两个文件描述符

	if (pipe(fd) == -1)//创建PIPE，并将文件描述符放进fd[2]中
	{
		perror("pipe()");
		exit(1);
	}

	pid_t x = fork();//创建一个子进程，将会继承PIPE的描述符

	if (x == 0)//子进程
	{
		char *s = "hello,I am your child\n";
		write(fd[1],s,strlen(s));//通过写端fd[1]将数据写入PIPE
	}

	if (x > 0)//父进程
	{
		char buf[30];
		bzero(buf,30);

		read(fd[0],buf,30);//通过读端fd[0]将数据从PEPE中读出
		printf("from child:%s\n",buf );
	}

	close(fd[0]);//关闭文件描述符
	close(fd[1]);
	return 0;
}