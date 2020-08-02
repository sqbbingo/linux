#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

extern char ** environ; //全局环境变量
int main(int argc, char const *argv[])
{
    int i;

    printf("argc = %d\n", argc);
    for (i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }
    printf("\n" );

    i = 0;
    while (environ[i])
        puts(environ[i++]);
    printf("\n" );

    return 0;
}