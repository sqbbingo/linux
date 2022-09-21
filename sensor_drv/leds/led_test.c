/*
* @Author: bingo
* @Date:   2022-08-29 22:21:51
* @Last Modified by:   bingo
* @Last Modified time: 2022-08-30 00:33:04
*/
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/ioctl.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include <poll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>

/*
* @description : main 主程序
* @param - argc : argv 数组元素个数
* @param - argv : 具体参数
* @return : 0 成功;其他 失败
*/
int main(int argc, char *argv[])
{
    int fd;
    char *filename;
    int databuf[3];
    unsigned short temp, light, distance;
    int ret = 0;

    if (argc != 3) {
        printf("Error Usage!\r\n");
        return -1;
    }

    filename = argv[1];
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        printf("can't open file %s\r\n", filename);
        return -1;
    }

    databuf[0] = atoi(argv[2]);
    printf("data1 = %d\n",databuf[0] );
    write(fd,databuf,1);

    close(fd); /* 关闭文件 */
    return 0;
}