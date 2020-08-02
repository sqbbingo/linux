/****************************************************************
先以可写方式打开当前目录下的“hello.txt”文件，如果该文件不存在，则创建文件，
再往文件中写入一个字符串“Hello, welcome to linux world!”，把操作结果输出到终端后，
关闭文件。接着再次以只读模式打开该文件，读取刚才写入的数据，并把结果输出到终端，
最后关闭该文件。
****************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    int fd;
    char file_name[] = "hello.txt";
    char data_write[] = "hello,welcome to linux world!";
    char data_read[100];

    fd = open(file_name, O_WRONLY | O_CREAT, 0X644);
    if (fd < 0)
    {
        perror("open file fail");
        return 0;
    }

    if (write(fd, data_write, sizeof(data_write)) < 0)
    {
        perror("write fail");
        close(fd);
        return 0;
    }
    fsync(fd);
    close(fd);

    fd = open(file_name, O_RDONLY);
    if (fd < 0)
    {
        perror("open file fail");
        return 0;
    }

    if (read(fd, data_read, 100) < 0)
    {
        perror("read file fail!");
        close(fd);
        return 0;
    }
    printf("file data : %s\n", data_read);
    close(fd);

    return 0;
}