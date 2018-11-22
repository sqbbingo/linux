#include <linux/module.h>	//内核模块必需包含的内核头文件，动态的将模块加载到内核中去，包含MODULE_LICESENCE()等宏定义
#include <linux/init.h>

static int_init hello_init(void)
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

MODULE_LICENSE("GPL");
