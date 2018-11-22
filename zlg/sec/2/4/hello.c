#include <linux/module.h>	//内核模块必需包含的内核头文件，动态的将模块加载到内核中去，包含MODULE_LICESENCE()等宏定义
#include <linux/init.h>		//内核模块必需包含的内核头文件

static int_init hello_init(void)	//声明为 static， 因为初始化函数对于其它文件没有任何意义，__init 表示初始化函数仅仅在初始化期间使用
{
	printk("Hello,I'm ready!\n");
	return 0;
}

static void_exit hello_exit(void)
{
	printk("I'll be leaving,bye!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");		//模块必须指定一个许可协议，否则会出错
