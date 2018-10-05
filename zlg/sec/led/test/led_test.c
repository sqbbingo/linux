#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <limits.h>
#include <asm/ioctls.h>
#include <time.h>
#include <pthread.h>


int main(void)
{
	int fd;
	char buf[1] = {0};

	fd = open("/dev/imx28x_led", O_RDWR);
	if (fd < 0) {
		perror("open /dev/imx283_led");
	}

	printf("test write....\n");
	buf[0] = 0;
	write(fd, buf, 1);
	printf("ON\n");
	sleep(2);

	buf[0] = 1;
	write(fd, buf, 1);
	printf("OFF\n");
	sleep(1);	

	printf("test ioctl..... \n");
	ioctl(fd, 0);
	printf("ON\n");
	sleep(2);
	ioctl(fd, 1);
	printf("OFF\n");
	printf("close\n");
}
