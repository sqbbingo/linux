/*
* @Author: bingo
* @Date:   2020-08-04 19:51:45
* @Last Modified by:   bingo
* @Last Modified time: 2020-08-04 20:13:17
* 首先创建共享内存，然后设置大小并映射共享内存，最后检测共享内存首字节数据是否为 18，如果不是，继续等待，否则打印显示，并
取消和删除共享内存
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SHMSIZE 10  //共享内存大小 10字节
#define SHMNAME "shmtest1"

int main(int argc, char const *argv[])
{
    int fd;
    char * ptr;

    //创建共享内存
    fd = shm_open(SHMNAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0)
    {
        perror("shm open error");
        exit(-1);
    }

    //映射共享内存
    ptr = mmap(NULL, SHMSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap error");
        exit(-1);
    }

    //设置内存大小
    ftruncate(fd, SHMSIZE);

    while (*ptr != 18)             //读数据
    {
        sleep(1);
    }
    printf("ptr:%d\n", *ptr);

    munmap(ptr, SHMSIZE);   //取消映射
    shm_unlink(SHMNAME);    //删除共享内存

    return 0;
}