/*
* @Author: bingo
* @Date:   2020-08-02 15:40:44
* @Last Modified by:   bingo
* @Last Modified time: 2020-08-02 15:53:14
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_exit_status(int status)
{
    if (WIFEXITED(status))  //正常退出，打印退出返回值
        printf("normal termination,exit status = %d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status)) //因信号异常退出，打印引起退出的信号
        printf("abnormal termination,signal number = %d\n", WTERMSIG(status));
    else
        printf("other status\n");
}

int main(int argc, char const *argv[])
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0) //创建子进程
    {
        perror("fork error");
        exit(-1);
    }
    else if (pid == 0)
    {
        exit(7);    //子进程调用exit函数
    }

    if (wait(&status) != pid)
    {
        perror("fork error");
        exit(-1);
    }
    print_exit_status(status);  //打印退出状态信号

    if ((pid = fork()) < 0)
    {
        perror("fork error");
        exit(-1);
    }
    else if (pid == 0)
    {
        abort();
    }

    if (wait(&status) != pid)   //父进程等待子进程退出，并获取退出状态
    {
        perror("fork error");
        exit(-1);
    }
    print_exit_status(status);  //打印第二个退出状态信息

    return 0;
}