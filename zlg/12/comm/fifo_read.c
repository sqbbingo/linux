/*
* @Author: bingo
* @Date:   2020-08-03 20:58:11
* @Last Modified by:   bingo
* @Last Modified time: 2020-08-03 21:14:41
* 通过命名管道获取数据并保存成文件的功能。程序打开命名管道文件/tmp/fifo，然后循环从命名管道中读取数据并将它们写入由参数给出的文件中
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    char fifo_name[] = "/tmp/fifo";
    int fifo_fd, data_fd;
    char buf[BUFSIZE];
    int datalen;

    if (argc != 2)
    {
        fprintf(stderr, "Usage %s <string>\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if ((fifo_fd = open(fifo_name, O_RDONLY)) < 0)//打开管道
    {
        perror("fifo open:");
        exit(EXIT_FAILURE);
    }

    if ((data_fd = open(argv[1], O_WRONLY | O_CREAT, 0664)) < 0)//打开目标文件
    {
        perror("data open:");
        exit(EXIT_FAILURE);
    }

    datalen = read(fifo_fd, buf, BUFSIZE);
    while (datalen > 0)
    {
        write(data_fd, buf, datalen);
        datalen = read(fifo_fd, buf, BUFSIZE);
    }

    close(fifo_fd);
    close(data_fd);

    return 0;
}