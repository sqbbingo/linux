#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char * env_init[] = {"USER=bingo", "HOME=/mnt/d/code/linux/zlg/12/exec", NULL};

int main(int argc, char const *argv[])
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        perror("fork error");
    }
    else if (pid == 0)
    {
        execle("/mnt/d/code/linux/zlg/12/exec/sample3", "hell0", "world", (char *)0, env_init); //子进程转载新进程
        perror("execle error");
        exit(-1);
    }
    else
    {
        exit(0);
    }

    return 0;
}