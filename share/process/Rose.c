#include "head4fifo.h"

int main(int argc, char const *argv[])
{
	if (access(FIFO,F_OK))
	{
		mkfifo(FIFO,0644);
	}

	int fifo = open(FIFO,O_RDONLY);//以只读方式打开管道

	char msg[20];
	bzero(msg,20);

	read(fifo,msg,20);//将数据从FIFO中读出
	printf("from FIFO:%s",msg );
	
	return 0;
}