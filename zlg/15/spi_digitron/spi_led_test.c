/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>

#include <linux/spi/spidev.h>

#define SPI_DEVICE	"/dev/spidev1.0"
#define GPIO_DEVICE 	"/sys/class/gpio/gpio117/value"

/* Nixie light value tale   0     1     2     3     4     5      6     7    8     9  */
int led_value_table[] =   {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90,
/*                          0.    1.    2.    3.    4.    5.     6.    7.   8.    9  */
			   0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

static uint8_t mode = 0;
static uint8_t bits = 8;
static uint32_t speed = 10000;
static uint16_t delay = 0;

static void show_led_num(int fd_spi, int fd_gpio, int value, int num)
{
	int ret;

	uint8_t tx[] = {led_value_table[value], (1 << num)};

	struct spi_ioc_transfer tr_txrx[] = {
		{
                .tx_buf      = (unsigned long)tx,
                .rx_buf      = 0,
                .len         = 2,
                .delay_usecs = delay,
                .speed_hz    = speed,
                .bits_per_word = bits,
		},
	};

        ret = ioctl(fd_spi, SPI_IOC_MESSAGE(1), &tr_txrx[0]);
        if (ret == 1) {
                printf("can't revieve spi message");
		return;
	}
	
	write(fd_gpio, "0", 1);
	usleep(100);
	write(fd_gpio, "1", 1);

}


int main(int argc, char *argv[])
{
	int ret       = 0;
	int fd_spi    = 0;
	int fd_gpio   = 0;
	int led_value = 0;
	int led_num   = 0;

	if (argc < 3) {
		printf("cmd : ./spi_led_test led_value led_num \n ");
		return -1;
	}

	led_value = atoi(argv[1]);
	if ((led_value) < 0 || (led_value > 19)) {
		printf("led num just in 0 ~ 19 (0 ~ 9 show 0 ~ 9 and 10 ~ 19 show 0.~ 9.) \n");	
		return -1;
	}

	led_num = atol(argv[2]);
	if ((led_num < 0) || (led_num > 3)) {
		printf("led number just in 0 ~ 3");
		return -1;
	}

	fd_spi = open(SPI_DEVICE, O_RDWR);
	if (fd_spi < 0) {
		printf("can't open %s \n", SPI_DEVICE);
		return -1;
	}

	fd_gpio = open(GPIO_DEVICE, O_RDWR);	
	if (fd_gpio < 0) {
		printf("can't open %s device\n", GPIO_DEVICE);
		return -1;
	}

	ret = ioctl(fd_spi, SPI_IOC_WR_MODE, &mode);			/* set spi mode 		*/
	if (ret == -1) {
		printf("can't set wr spi mode\n");
		return -1;
	}

	ret = ioctl(fd_spi, SPI_IOC_WR_BITS_PER_WORD, &bits);		/* set spi bits per word	*/
	if (ret == -1) {
		printf("can't set bits per word\n");
		return -1;
	}

	ret = ioctl(fd_spi, SPI_IOC_WR_MAX_SPEED_HZ, &speed);		/* max speed hz			*/
	if (ret == -1)	{
		printf("can't set max speed hz\n");
		return -1;
	}

	show_led_num(fd_spi, fd_gpio, led_value, led_num);

	close(fd_spi);

	return ret;
}
