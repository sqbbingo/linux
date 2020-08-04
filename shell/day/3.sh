#!/bin/sh
#每周五使用tar命令备份文件
#备份文件名需包含日期标签
#注意date命令需要使用反括号或者$()包含
tar -czPf /home/bingo/Desktop/code/shell/day/log-`date +%Y%m%d`.tar.gz /home/bingo/Desktop/code/shell/day/2.sh

#crontab -e #编写计划任务执行脚本
00	03	*	*	5	/home/bingo/Desktop/code/shell/day/3.sh
