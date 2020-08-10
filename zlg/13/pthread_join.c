/*
* @Author: bingo
* @Date:   2020-08-09 10:47:03
* @Last Modified by:   bingo
* @Last Modified time: 2020-08-09 11:04:53
* 主线程创建了 4 个线程来进行数学运算，
* 每个线程将运算将结果都通过 pthread_exit()函数返回给主线程，
* 主线程使用 pthread_join()来等待 4 个线程完成并获取它们的运行结果。
*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

#define PTHREAD_NUM 4

void * pthrea_runing(void * id)
{
    long tid;
    tid = (long)id;
    int i;
    double result = 0.0;

    printf("Thread %ld starting...\n", tid );
    for (i = 0; i < 1000000; i++)
    {
        result = result + sin(i) * tan(i);
    }
    printf("Thread %ld done.Result = %e\n", tid, result );
    pthread_exit((void*)id);
}

int main(int argc, char const *argv[])
{
    long i;
    pthread_t pth[PTHREAD_NUM];
    int state;
    void * status;


    for (i = 0; i < PTHREAD_NUM; i++)
    {
        printf("Main creating pthread :%ld\n", i );
        state = pthread_create(&pth[i], NULL, pthrea_runing, (void *)i);
        if (state)
        {
            printf("main create pthread %ld error:%d \n", i, state );
            exit(-1);
        }
    }

    for (i = 0; i < PTHREAD_NUM; i++)
    {
        state = pthread_join(pth[i], &status);
        if (state)
        {
            printf("error;return code from pthread_join is %d\n", state);
            exit(-1);
        }
        printf("Main:completed join with Thread %ld having a status of %ld\n", i, (long)status );
    }
    printf("Main:program completed.Exiting\n");
    pthread_exit(NULL);

    return 0;
}