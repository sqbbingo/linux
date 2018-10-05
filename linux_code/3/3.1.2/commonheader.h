#ifndef _COMMONHEADER_H_
#define _COMMONHEADER_H_

#include <stdio.h>//1.普通操作
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <errno.h>

#include <sys/stat.h>//2.文件于目录操作
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>

#include <sys/ipc.h>//3.进程间通信
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <signal.h>

#include <pthread.h>//4.线程

#include <arpa/inet.h>//5.网络
#include <netinet/in.h>
#include <sys/socket.h>

#endif