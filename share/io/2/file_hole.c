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
	int fd = open("file.txt",O_RDWR|O_CREAT|O_TRUNC,0644);

	write(fd,"abc",3);		//写入"abc"
	lseek(fd,100,SEEK_CUR);//定位到100个字节之后
	write(fd,"xyz",3);		//写入"xyz"

	close(fd);
	return 0;
}