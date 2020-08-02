#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>	//设备号相关

#define DEVICE_NAME		"char_null"
static int major = 0;		//保存主设备号的全局变量

static int __init char_null_init(void)
{
	int ret;

	ret = register_chrdev(major,DEVICE_NAME,&major);	//申请设备号和注册
	if(major > 0){	//静态设备号
		if(ret < 0){
			printk(KERN_INFO"Can't get major number!\n");
			return ret;
		}
	}else{		//动态设备号
		printk(KERN_INFO"ret is %d\n",ret);
		major = ret;		//保存动态获取的主设备号
	}
	printk(KERN_INFO"%s ok!\n",__func__);
	return ret;
}

static void __exit char_null_exit(void)
{
	unregister_chrdev(major,DEVICE_NAME);
	printk(KERN_INFO"%s\n",__func__);
}

module_init(char_null_init);
module_exit(char_null_exit);

MODULE_LICENSE("GPL");
