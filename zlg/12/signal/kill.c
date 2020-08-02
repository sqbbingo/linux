/*
* @Author: bingo
* @Date:   2020-08-02 16:46:07
* @Last Modified by:   bingo
* @Last Modified time: 2020-08-02 16:55:40
* 演示了 kill 函数的用法，父进程向子进程发生 SIGINT 信号，并使用 wait函数获取子进程的退出状态
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void print_exit_status(int status)
{
    if (WIFEXITED(status))
        printf("normal termination,exit status = %d\n", WEXITSTATUS(status) );
    else if (WIFSIGNALED(status))
        printf("abnormal terminaton,signal number = %d\n", WTERMSIG(status));
    else
        printf("other status\n");
}

int main(int argc, char const *argv[])
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
    {
        perror("fork error");
        exit(-1);
    }
    else if (pid == 0)
    {
        while (1)
        {
            printf("child sleeping\n");
            sleep(1);
        }
        exit(0);
    }
    else
    {
        sleep(2);
        printf("parent send SIGINT to child\n");
        kill(pid, SIGINT);
        if (wait(&status) != pid)
        {
            perror("wait error");
            exit(-1);
        }
        print_exit_status(status);
    }

    return 0;
}