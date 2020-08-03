/*
* @Author: bingo
* @Date:   2020-08-03 19:37:49
* @Last Modified by:   bingo
* @Last Modified time: 2020-08-03 20:00:25
* 使用匿名管道方式实现了向子进程传递字符串的功能。由于管道传输是半双工的，数据只能在单个方向上流动，父进程往子进程传数据时，父进程的管道读端和子
进程的管道写端都可以先关闭
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t cpid;
    char buf;
    int pipefd[2];

    if (argc != 2)
    {
        fprintf(stderr, "Usage %s <string>\n", argv[0] );
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1) //创建匿名管道
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if ((cpid = fork()) == -1)  //创建子进程
    {
        perror("create fork:");
        exit(EXIT_FAILURE);
    }
    else if (cpid == 0)
    {   //子进程读管道读端
        close(pipefd[1]);                   //关闭不需要的写端
        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(pipefd[0]);
        sleep(10);
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    return 0;
}