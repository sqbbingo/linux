/*
* @Author: bingo
* @Date:   2022-08-23 23:23:48
* @Last Modified by:   bingo
* @Last Modified time: 2022-08-23 23:25:55
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
    unsigned short databuf[3];
    unsigned short temp, light, distance;
    int ret = 0;

    if (argc != 2) {
        printf("Error Usage!\r\n");
        return -1;
    }

    filename = argv[1];
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        printf("can't open file %s\r\n", filename);
        return -1;
    }

    while (1) {
        ret = read(fd, databuf, sizeof(databuf));
        if (ret == 0) { /* 数据读取成功 */
            temp = databuf[0]; /* temp 传感器数据 */
            light = databuf[1]; /* light 传感器数据 */
            distance = databuf[2]; /* distance 传感器数据 */
            printf("temp = %d, light = %d, distance = %d\r\n", temp, light, distance);
        }
        usleep(200000); /* 200ms */
    }
    close(fd); /* 关闭文件 */
    return 0;
}