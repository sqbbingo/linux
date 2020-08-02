/*
* @Author: bingo
* @Date:   2020-08-02 16:36:00
* @Last Modified by:   bingo
* @Last Modified time: 2020-08-02 16:44:18
* 实现当进程首次收到 SIGINT(Ctrl+c)信号时在终端打印 Ouch 信息，后续又恢复到默认的处理方法。
*/
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void ouch(int sig)
{
    printf("\nOuch! -I got singal %d\n", sig);
}

int main(int argc, char const *argv[])
{
    struct sigaction act;

    act.sa_handler = ouch;   //设置信号处理函数
    sigemptyset(&act.sa_mask);  //清空屏蔽信号集
    act.sa_flags = SA_RESETHAND;    //设置信号处理之后恢复默认的处理方式

    sigaction(SIGINT, &act, NULL);  //设置SIGINT信号的处理方式

    while (1)
    {
        printf("sleeping\n");
        sleep(1);
    }


    return 0;
}