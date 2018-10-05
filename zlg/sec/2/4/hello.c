#include <linux/module.h>
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
