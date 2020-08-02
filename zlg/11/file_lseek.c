/*********************************
所读写的文件是由前面 file_wr 程序创建的“hello.txt”。
在这个例子中，通过 lseek()设置读写位置，首先从文件开始的地方读 6 个字节，接着用 lseek()
从当前位置（偏移 5）移动到偏移 18 的地方，读取从偏移 18 到文件结尾之间的数据，再用
lseek()把读写位置移动到从结尾开始，往文件开头方向第 7 字节处（反偏移-7），调用 read()
读取反偏移-7 到文件结尾之间的数据
*******************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    int fd;
    int res;
    char file_name[] = "hello.txt";
    char str_read[50];

    //打开文件
    fd = open(file_name, O_RDONLY);
    if (fd < 0)
    {
        perror("open file fail:");
        goto END;
    }

    //读取从文件开头的6个字符
    lseek(fd, 0, SEEK_SET);
    if ((res = read(fd, str_read, 5)) < 0)
    {
        perror("read 0~6 fail");
        goto END;
    }
    str_read[res] = '\0';
    printf("0~6:%s\n", str_read);

    //读取从18到尾部的字符
    lseek(fd, 12, SEEK_CUR);
    if ((res = read(fd, str_read, 30)) < 0)
    {
        perror("read 18~end fail");
        goto END;
    }
    str_read[res] = '\0';
    printf("18~END:%s \n", str_read);

    //读取从-7到尾部的字符
    lseek(fd, -7, SEEK_END);
    if (read(fd, str_read, 10) < 0)
    {
        perror("read -7~end:fail");
        goto END;
    }
    printf("-7~END:%s \n", str_read);

END:
    close(fd);

    return 0;
}