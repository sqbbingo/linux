#ifndef __KS103_H
#define __KS103_H
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名 : KS103.h
作者 : 左忠凯
版本 : V1.0
描述 : KS103 寄存器地址描述头文件
其他 : 无
论坛 : www.openedv.com
日志 : 初版 V1.0 2019/9/2 左忠凯创建
***************************************************************/
/* AP3316C 寄存器 */
#define KS103_SYSTEMCONG 0x00 /* 配置寄存器 */
#define KS103_INTSTATUS 0X01 /* 中断状态寄存器 */
#define KS103_INTCLEAR 0X02 /* 中断清除寄存器 */
#define KS103_IRDATALOW 0x0A /* IR 数据低字节 */
#define KS103_IRDATAHIGH 0x0B /* IR 数据高字节 */
#define KS103_ALSDATALOW 0x0C /* ALS 数据低字节 */
#define KS103_ALSDATAHIGH 0X0D /* ALS 数据高字节 */
#define KS103_PSDATALOW 0X0E /* PS 数据低字节 */
#define KS103_PSDATAHIGH 0X0F /* PS 数据高字节 */

#endif