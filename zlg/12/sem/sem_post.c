/*
* @Author: bingo
* @Date:   2020-08-08 16:16:11
* @Last Modified by:   bingo
* @Last Modified time: 2020-08-08 16:27:38
* 程序获取参数打开指定的共享内存，并设置大小和完成
映射；然后打开服务端器已经创建的信号量，从标准输入读入一串字符串后写入共享内存区，
然后使用 sem_post()函数通知服务端
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define MAPSIZE 100
int main(int argc, char const *argv[])
{
    int shmid;
    char * ptr;
    sem_t * semid;

    if (argc != 2)
    {
        printf("Usage:%s <pathname>\n", argv[0] );
        return -1;
    }

    shmid = shm_open(argv[1], O_RDWR, 0); //打开共享内存对象
    if (shmid == -1)
    {
        printf("open shared memory error\n");
        return -1;
    }
    ftruncate(shmid, MAPSIZE);
    ptr = mmap(NULL, MAPSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);

    semid = sem_open(argv[1], 0); //打开信号量对象
    if (semid == SEM_FAILED)
    {
        printf("open semaphore error\n");
        return -1;
    }

    printf("client input:\n");
    fgets(ptr, MAPSIZE, stdin); //从标准输入读取需要写入共享内存的值
    sem_post(semid);    //通知服务器

    munmap(ptr, MAPSIZE);   //取消对共享内存的映射
    close(shmid);
    sem_close(semid);

    return 0;
}