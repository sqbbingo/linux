#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x405fe887, "module_layout" },
	{ 0x564a6735, "device_create" },
	{ 0x582a74ba, "__class_create" },
	{ 0xc81bb072, "cdev_add" },
	{ 0x8e74204, "cdev_init" },
	{ 0xbd68de57, "cdev_alloc" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xfe990052, "gpio_free" },
	{ 0x42b26667, "class_destroy" },
	{ 0xd5c45942, "device_destroy" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x73ff3ad4, "cdev_del" },
	{ 0x432fd7f6, "__gpio_set_value" },
	{ 0x43b0c9c3, "preempt_schedule" },
	{ 0xea147363, "printk" },
	{ 0xa8f59416, "gpio_direction_output" },
	{ 0xd49c12f1, "module_put" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

