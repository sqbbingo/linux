#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

extern char ** environ;

int main(int argc, char const *argv[])
{
    pid_t pid;

    pid = getpid();
    printf("id = %d\n", pid);
    printf("pid = %d\n", getppid() );

    int i = 0;
    while (environ[i])
        puts(environ[i++]);

    char * env;
    env = getenv("HOME");
    printf("HOME:%s\n", env);

    //创建进程
    pid = fork();
    if (pid == 0)
    {
        printf("Here is children id = %d,parent id = %d\n", getpid(), getppid() );
    }
    else if (pid > 0)
    {
        printf("Here is parent id = %d,children id = %d\n", getpid(), pid );
    }
    else
    {
        perror("fork fail");
    }

    return 0;
}