#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	if (argc != 3)
	{
		printf("Usage:%s<src> <dst>\n",argv[0] );
		exit(1);
	}

	//分别以只读和只写模式打开源文件和目标文件
	FILE *fp_src = fopen(argv[1],"r");
	FILE *fp_dst = fopen(argv[2],"w");

	//如果返回NULL则程序出错退出
	if (fp_src == NULL || fp_dst == NULL)
	{
		perror("fopen()");
		exit(1);
	}

	int c,total = 0;
	while(1)
	{
		c = fgetc(fp_src);//从源文件读取一个字符存储在变量c中

		if (c == EOF && feof(fp_src))//已达文件末尾
		{
			printf("copy completed,""%d bytes have been copied.\n",total);
			break;
		}
		else if (ferror(fp_src))//遇到错误
		{
			perror("fgetc()");
			break;
		}

		fputc(c,fp_dst);//将变量c中的字符写入目标文件中
		total++;//累计拷贝字符个数
	}

	//正常关闭文件指针，释放系统资源
	fclose(fp_src);
	fclose(fp_dst);

	return 0;
}