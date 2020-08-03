/*
* @Author: bingo
* @Date:   2020-08-03 20:15:46
* @Last Modified by:   bingo
* @Last Modified time: 2020-08-03 20:46:19
* 程序先判断命名管道是否已存在，如果不存在则创建命名管道/tmp/fifo 文件，然后将参数所指出文件的数据循环读出并写进命名管道中
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    char fifo_name[] = "/tmp/fifo";
    int pipefd, datafd;
    char buf[BUFSIZE];
    int datalen;

    if (argc != 2)  //检查是否参数是否带文件名
    {
        fprintf(stderr, "Usage %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (access(fifo_name, F_OK) == 0)    //检查管道是否存在
    {
        perror("fifo file:");
        exit(EXIT_FAILURE);
    }

    if (mkfifo(fifo_name, 0777) < 0)   //创建有名管道
    {
        perror("create fifo:");
        exit(EXIT_FAILURE);
    }

    if ((pipefd = open(fifo_name, O_WRONLY)) < 0)   //打开管道
    {
        perror("pipefd:");
        exit(EXIT_FAILURE);
    }

    if ((datafd = open(argv[1], O_RDONLY)) < 0) //打开源数据文件
    {
        perror("datafd:");
        exit(EXIT_FAILURE);
    }

    datalen = read(datafd, buf, BUFSIZE);   //读取文件写入管道
    while ( datalen > 0)
    {
        write(pipefd, buf, datalen);
        datalen = read(datafd, buf, BUFSIZE);
    }

    close(pipefd);
    close(datafd);

    return 0;
}