#include <stdio.h>
// #include <sys/types.h>
// #include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(void)
{
	int fd, fd1;
	char buf[100];
	int buf2[20];

	fd = open("test.txt", O_RDWR);
	if (fd < 0)
	{

		perror("open test.txt fail!\n");
	}
	else
	{
		printf("open test.txt success!\n");
	}

	if (read(fd, buf, 20) < 0)
	{
		perror("read fail");
		printf("%d\n", errno );
	}
	else
	{
		printf("read:%s,%ld,%ld\n", buf, sizeof(buf2) , strlen(buf) );
	}

	char data[] = "this is test";

	if (write(fd, data, sizeof(data)) < 0)
	{
		perror("write fail");
	}
	else
	{
		fsync(fd);
		lseek(fd, 0, SEEK_SET);
		read(fd, buf, 100);
		printf("write success %s\n", buf);
	}


	close(fd);


	return 0;
}
