/*
* @Author: suqibin
* @Date:   2020-08-04 19:27:57
* @Last Modified by:   bingo
* @Last Modified time: 2020-08-04 20:11:20
* 先创建共享内存，设置大小并完成映射，随后往共享内存起始地址写入一个值为 18 的整形数据，最后取消和删除共享内存
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
    fd = shm_open(SHMNAME, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0)
    {
        perror("shm open error");
        exit(-1);
    }

    //设置内存大小
    ftruncate(fd, SHMSIZE);
    //映射共享内存
    ptr = mmap(NULL, SHMSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap error");
        exit(-1);
    }

    *ptr = 18;              //往起始地址写入数据
    munmap(ptr, SHMSIZE);   //取消映射
    shm_unlink(SHMNAME);    //删除共享内存

    return 0;
}