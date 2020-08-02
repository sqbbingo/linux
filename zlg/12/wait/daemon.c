/*
* @Author: bingo
* @Date:   2020-08-02 16:13:24
* @Last Modified by:   bingo
* @Last Modified time: 2020-08-02 16:19:44
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    int fd;
    time_t curtime;

    if (daemon(0, 0) == -1)
    {
        perror("daemon error");
        exit(-1);
    }

    fd = open("/mnt/d/code/linux/zlg/12/wait/daemon.log", O_WRONLY | O_CREAT | O_APPEND, 0X664);
    if (fd < 0)
    {
        perror("open error");
        exit(-1);
    }

    while (1)
    {
        curtime = time(0);
        char *timestr = asctime(localtime(&curtime));
        write(fd, timestr, strlen(timestr));
        sleep(60);
    }

    close(fd);

    return 0;
}