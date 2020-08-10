/*
* @Author: bingo
* @Date:   2020-08-09 11:20:13
* @Last Modified by:   bingo
* @Last Modified time: 2020-08-10 08:32:49
* 主线程根据参数给出的线程栈大小来设置线程属性对象，
* 然后使用剩余参数分别创建线程来实现小写转大写的功能并打印出栈地址
*/
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>