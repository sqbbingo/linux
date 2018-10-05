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
	if(argc != 3)//输入两个参数，用法类似于shell命名：cp file1 file2
	{
		printf("Usage:%s<src> <dst>\n",argv[0] );
		exit(1);
	}

	//创建一个子进程
	pid_t a = fork();

	//父子进程都打开源文件和目标文件
	int fd1 = open(argv[1],O_RDONLY);
	int fd2 = open(argv[2],O_CREAT|O_RDWR_O_TRUNC,0644);
	if (fd1 == -1 || fd2 == -1)
	{
		perror("open()");
		exit(1);
	}

	int size = lseek(fd1,0,SEEK_END);//获得文件大小
	if (a == 0)//在子进程中，将位置偏移量调整到中间位置（形成空洞）
	{
		lseek(fd1,size/2,SEEK_SET);
		lseek(fd2,size/2,SEEK_SET);
	}
	else if(a > 0)//在父进程中，将文件位置偏移量调整到文件开头处
	{
		lseek(fd1,0,SEEK_SET);
	}

	char buf[100];
	int nread;

	while(1)
	{
		bzero(buf,100);
		nread = read(fd1,buf,100);
		if(nread == 0)
			break;
		if (a > 0)
		{
			//在父进程中，查看当前偏移量是否已经到达中间位置
			int n;
			n = lseek(fd1,0,SEEK_CUR) - size/2;
			if (n >= 0)//到达甚至已经超过中间位置
			{
				write(fd2,buf,n);//写入超过中间位置的字节
				exit(0);//然后退出
			}
		}

		write(fd2,buf,nread);
	}

	close(fd1);
	close(fd2);


	
	return 0;
}