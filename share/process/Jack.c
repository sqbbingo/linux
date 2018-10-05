#include "head4fifo.h"

int main(int argc, char const *argv[])
{
	if (access(FIFO,F_OK))
	{
		mkfifo(FIFO,0644);
	}

	int fifo = open(FIFO,O_WRONLY);//以只写方式打开FIFO

	char msg[20];
	bzero(msg,20);

	fgets(msg,20,stdin);
	int n = write(fifo,msg,strlen(msg));//将数据写入FIFO

	printf("%dbytes have been sended\n", n);
	return 0;
}