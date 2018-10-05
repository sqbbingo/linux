#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(void)
{
	int fd = open("a.txt",O_CREAT|O_TRUNC|O_WRONLY,0644);
	printf("fd:%d\n",fd);

	close(fd);
	return 0;
}
