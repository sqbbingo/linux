#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	FILE *fp = fopen("a.txt","r+");//以读写方式打开已存在文件

	//如果打开文件a.txt失败，fopen()将会返回NULL
	if(fp == NULL)
	{
		perror("fopen");
		exit(1);
	}

	//如果关闭fp失败，fclose()将会返回EOF
	if (fclose(fp) == EOF)
	{
		perror("fclose()");
		exit(1);
	}
	
	return 0;
}