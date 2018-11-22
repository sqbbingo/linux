#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

int main (int argc, char * argv[])
{
        int fd,count;
        struct input_event input_event_value;
        char input_type[20];

        if (argc !=2 ) {
                /* 判断程序是否有输入参数,如果没有程序退出 */
                printf("usage : input_type /dev/input/eventX\n");
                return 0;
        }

        fd = open (argv[1], O_RDWR);
        /* 打开输入设备,设备的名称为程序的输入参数提供 */
        if (fd < 0) {
                printf ("open %s failed\n", argv[1]);
                exit(0);
        }

        /* 循环读取输入事件,然后打印事件信息 */
        while(1) {
                count=read(fd, &input_event_value, sizeof(struct input_event));
                if (count < 0) {
                        printf("read iput device event error \n");
                        return -1;
                }
                switch(input_event_value.type) {
                        /* 判断事件的类型*/
                        case EV_SYN:
                        strcpy(input_type, "SYNC");
                        break;
                        case EV_REL:
                        strcpy(input_type, "REL");
                        break;
                        case EV_ABS:
                        strcpy(input_type, "ABS");
                        break;
                        case EV_KEY:
                        strcpy(input_type, "KEY");
                        break;
                        default:
                        printf("even type unkown \n");
                        return -1;
                }
                /* 打印输入事件的时间*/
                printf("time:%ld.%ld",input_event_value.time.tv_sec,input_event_value.time.tv_usec);
                /* 打印输入事件的类型、码值、value 值 */
                printf(" type:%s code:%d value:%d\n",input_type,input_event_value.code,input_event_value.value);
        }
        
        return 0;
}