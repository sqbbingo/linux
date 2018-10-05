#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define SIZE 1024

int main(int argc,char **argv)
{
	int fd_from,fd_to;

	if(argc != 3)
	{
		printf("Uage:%s <src> <dst>",argv[0]);
		exit(1);
	}
	
	fd_from = open(argv[1],O_RDONLY);
	fd_to = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644);

	char buf[SIZE];
	char *p;
	int nread,nwrite;

	while(1)
	{
		//如果出错而且错误码是EINTR,则循环再读
		while((nread=read(fd_from,buf,SIZE))==-1 && errno == EINTR)
		{
			;
		}	

		//如果出错但错误码不是EINTR,则遇到真正的错误，退出
		if (nread == -1)
		{
			perror("read() errno");
			break;
		}

		if(nread == 0)
		{
			break;//nread为0代表读到了文件尾，拷贝完成
		}
		p = buf;
		while(nread > 0)
		{
			//如果出错且错误码是EINTR，则循环再写
			while((nwrite=write(fd_to,p,SIZE))==-1 && errno == EINTR)

				//否则遇到真正错误，退出
				if (nwrite == -1)
				{
					perror("write()errno");
					break;
				}

				nread -= nwrite;//nread减去已写入的nwrite
				p += nwrite;//调整写入数据的指针
		}
	}
	
	close(fd_from);
	close(fd_to);
	
	return 0;
}
