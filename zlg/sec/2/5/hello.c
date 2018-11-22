#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/bcd.h>
#include <linux/capability.h>
#include <linux/rtc.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>

static int num = 3;
static char *whom = "master";

module_param(num,int,S_IRUGO);
module_param(whom,charp,S_IRUGO);

static int __init hello_init(void)
{
	printk(KERN_INFO"%s,I get%d\n",whom,num);
	return 0;
}

static void __exit hello_exit(void)
{
	printk("I'll be leaving,bye!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
