/*
* @Author: suqibin
* @Date:   2020-08-08 14:57:15
* @Last Modified by:   suqibin
* @Last Modified time: 2020-08-08 16:11:15
* 程序从启动参数获取共享内存名称，然后创建共享
内存对象，设置大小后完成映射；最后创建信号量对象并等待客户端的通知，其中信号量的
初始值为 0。在共享内存映射后使用 sem_wait()函数等待客户端完成对共享内存的写入操作
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define MAPSIZE 100 //共享内存大小

int main(int argc, char const *argv[])
{
    /* code */
    int shmid;
    sem_t * semid;
    char * ptr;

    if (argc != 2)
    {
        printf("usage :%s <pathname>\n", argv[0]);
        return -1;
    }

    shmid = shm_open(argv[1], O_RDWR | O_CREAT, 0644); //创建共享内存对象
    if (shmid == -1)
    {
        printf("open shared memory error\n");
        return -1;
    }
    ftruncate(shmid, MAPSIZE);  //设置共享内存大小
    ptr = mmap(NULL, MAPSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0); //将共享内存进行映射
    strcpy(ptr, "\0");

    semid = sem_open(argv[1], O_CREAT, 0644, 0);
    if (semid == SEM_FAILED)
    {
        printf("open semaphore error \n");
        return -1;
    }
    sem_wait(semid);    //信号量等待操作，等待客户端修改共享内存
    printf("server recv:%s\n", ptr);//从共享内存中读取值
    strcpy(ptr, "\0");

    munmap(ptr, MAPSIZE);   //取消对共享内存的映射
    close(shmid);   //关闭共享内存
    sem_close(semid);   //关闭信号量

    sem_unlink(argv[1]);    //删除信号量对象
    shm_unlink(argv[1]);    //删除共享内存对象

    return 0;
}