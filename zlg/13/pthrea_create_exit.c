/*
* @Author: bingo
* @Date:   2020-08-09 10:14:19
* @Last Modified by:   bingo
* @Last Modified time: 2020-08-09 11:11:34
* 主线程创建了 5 个线程，这 5 个线程和主线程并发执行，
* 主线程创建完线程后调用 pthread_exit()函数退出线程，其它线程分别打印当前线程的序号。
* 当主线程先于其它进程执行 pthread_exit()时，进程还不会退出，只有最后一个线程也完成了，进程才会退出
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define PTHREAD_NUM 5

void * pthread_message(void * id)//线程函数
{
    long tid;
    tid = (long)id;
    printf("this is pthread %ld\n", tid);//打印线程对应的参数
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    long i;
    pthread_t pthread_id[PTHREAD_NUM];
    int state;

    printf("%ld,%ld\n", sizeof(void *), sizeof(long) );
    for (i = 0; i < PTHREAD_NUM; i++)//循环创建5个线程
    {
        printf("In main:creating thread %ld \n", i);
        state = pthread_create(&pthread_id[i], NULL, pthread_message, (void *)i);
        if (state)
        {
            printf("ERROR:return code from pthread_create is %d\n", state);
            exit(-1);
        }
    }
    printf("In main:exit!\n");
    pthread_exit(NULL);//主线程退出

    return 0;
}