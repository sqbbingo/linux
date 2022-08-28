/*
* @Author: bingo
* @Date:   2022-08-24 23:30:31
* @Last Modified by:   bingo
* @Last Modified time: 2022-08-25 00:02:04
*/
#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
 
#define MYLED_CNT       2           /* 设备号个数 */
#define MYLED_NAME      "myled"     /* 名字 */
 
#define LED_ON          1
#define LED_OFF         0
 
dev_t parent_devid;         /* 设备号 */
struct class *myled_class;  /* 类 */
 
/* LED设备结构体 */
struct myled_dev {
    dev_t devid;            /* 设备号 */
    struct cdev cdev;       /* cdev结构体 */
    struct device *device;  /* 设备 */
    int led_gpio;           /* GPIO号 */
    struct device_node *nd; /* 设备节点 */
                            /* 中断号 */
};
 
static struct myled_dev myled[MYLED_CNT];       /* led设备 */
 
/*
 * @description     : 打开设备
 * @param – inode   : 传递给驱动的inode
 * @param - filp    : 设备文件，file结构体有个叫做private_data的成员变量
 *                    一般在open的时候将private_data指向设备结构体。
 * @return          : 0 成功;其他 失败
 */
static int myled_open(struct inode *inode, struct file *filp)
{
    return 0;
}
 
/*
 * @description     : 向设备写数据 
 * @param – filp    : 设备文件，表示打开的文件描述符
 * @param - buf     : 要写给设备写入的数据
 * @param - cnt     : 要写入的数据长度
 * @param - offt    : 相对于文件首地址的偏移
 * @return          : 写入的字节数，如果为负值，表示写入失败
 */
static ssize_t myled_write(struct file *filp, const char __user *buf, 
            size_t cnt, loff_t *offt)
{
    int ret;
    char kern_buf[1];
 
    ret = copy_from_user(kern_buf, buf, cnt);   // 得到应用层传递过来的数据
    if(0 > ret) {
        printk(KERN_ERR "myled: Failed to copy data from user buffer\r\n");
        return -EFAULT;
    }
 
    // if (0 == kern_buf[0])
    //  gpio_set_value(myled.led_gpio, 0);      // 如果传递过来的数据是0则关闭led
    // else if (1 == kern_buf[0])
    //  gpio_set_value(myled.led_gpio, 1);      // 如果传递过来的数据是1则点亮led
 
    return 0;
}
 
static int myled_init(struct device_node *nd)  /* 这里写 uart引脚的一些初始化 */
{
    const char *str;                                                /* 1. 获取到父节点，可以直接拿到子节点的属性么 ？不能 */
    int val;                                                        /* 2. 先把属性保存到第一个设备里面 */
    int ret;                                                        /* 3. 标签重复了 ？ 是不是不知道哪个子节点 ？ 对不能访问 */
 
    /* 从设备树中获取GPIO */
    myled[0].led_gpio = of_get_named_gpio(nd, "led-gpio", 0);       /* 1. 首先你能跨子节点搜索么 ？ 不能 */
    if(!gpio_is_valid(myled[0].led_gpio)) {                         /* 2. of_path找到节点 */
        printk(KERN_ERR "myled: Failed to get led-gpio\n");         /* 3. 那肯定是一个设备一个初始化 */
        return -EINVAL;
    }
 
    /* 申请使用GPIO */
    ret = gpio_request(myled[0].led_gpio, "PS_LED0 Gpio");
    if (ret) {
        printk(KERN_ERR "myled: Failed to request led-gpio\n");
        return ret;
    }
 
    /* 确定LED初始状态 */
    ret = of_property_read_string(nd, "default-state", &str);
    if(!ret) {
        if (!strcmp(str, "on"))
            val = 1;
        else
            val = 0;
    } else
        val = 0;
 
    /* 将GPIO设置为输出模式并设置GPIO初始电平状态 */
    gpio_direction_output(myled[0].led_gpio, val);
 
    return 0;
}
 
/* LED设备操作函数 */  /* 可根据需要进行初始化 */
static struct file_operations myled_fops = {
    .owner = THIS_MODULE,
    .open = myled_open,
    .write = myled_write,
 
};
 
/*
 * @description     : platform驱动的probe函数，当驱动与设备
 *                    匹配成功以后此函数就会执行
 * @param - pdev    : platform设备指针
 * @return          : 0，成功;其他负值,失败
 */
static int myled_probe(struct platform_device *pdev)
{
    int ret, i;
    int major_devid;
    struct device_node *parent_nd;
 
    printk(KERN_INFO "myled: led driver and device has matched!\r\n");
 
    /* 使用of_path 获取父设备节点 */
    // parent_nd = of_find_node_by_path("/tasson_leds");    /* 可以通过路径获取到父设备节点 已验证 0219lc */
 
    /* 通过路径获取子设备节点 */
    myled[0].nd = of_find_node_by_path("/tasson_leds/gpio-led0");   
 
    /* led初始化 */   /* 这个初始化应该需要自己写寄存器 0219lc */
    ret = myled_init(myled[0].nd);    /* pdev->dev.of_node 获取设备大节点tasson_leds，  0219lc */
    if (ret)
        return ret;
 
    /* 申请父设备号，（为了创建子设备号） */
    ret = alloc_chrdev_region(&parent_devid, 0, MYLED_CNT, MYLED_NAME);     /* 该名字在 cat /proc/devices 下面可看到设备号 */
    if (ret)
        printk("alloc_chrdev_region fail ! \r\n");
//      goto out1;
    major_devid = MAJOR(parent_devid);
    printk("request major_devid is %d \r\n", major_devid);
 
    /* 给每个设备设置设备号，添加内核，0219lc */
    for ( i = 0; i < MYLED_CNT; i++)
    {
        /* 设置次设备号 */
        myled[i].devid = parent_devid + i;      
 
        /* 初始化cdev */
        myled[i].cdev.owner = THIS_MODULE;
        cdev_init(&myled[i].cdev, &myled_fops);     /* 每个设备都是绑定同一个 fops 操作函数结构体 0219lc */
 
        /* 添加cdev ，设备添加到内核 */
        ret = cdev_add(&myled[i].cdev, myled[i].devid, 1);  /* 此处是固定的一个一个添加 0219lc */
        if (ret)
            goto out2;
    }
 
    /* 创建类class ，无论是一个设备还是多个设备类只有一个 0219lc */       /* 这个创建类 能放到前面么 ？ 就可以把创建设备包含在上面哪个for循环了 ？？ 0219lc */
    myled_class = class_create(THIS_MODULE, MYLED_NAME);
    if (IS_ERR(myled_class)) {
        ret = PTR_ERR(myled_class);
        goto out3;
    }
 
    /* 创建设备 */
    for ( i = 0; i < MYLED_CNT; i++)
    {
        myled[i].device = device_create(myled_class, &pdev->dev,
                    myled[i].devid, NULL, "%s%d",MYLED_NAME, i);        /* 给设备组成名字的方式 0219lc */
        if (IS_ERR(myled[i].device)) {
            ret = PTR_ERR(myled[i].device);
            goto out4;
        }
    }
    
    printk("load module success! \r\n");       /* 不用加内核打印 KERN_INFO，也可以在终端打印信息 0219lc  */
 
    return 0;
 
out4:
    class_destroy(myled_class);
 
out3:
    for ( i = 0; i < MYLED_CNT; i++)
    {
        cdev_del(&myled[i].cdev);
    }
    
out2:
    unregister_chrdev_region(parent_devid, MYLED_CNT);  /* 这就是把申请的俩个设备号都注销掉 0219lc */
 
// out1:
//  gpio_free(myled.led_gpio);  /* 申请使用几个gpio引脚 ？ 设备是一个使用，同时使用 0219lc */
 
    return ret;
}
 
/*
 * @description     : platform驱动模块卸载时此函数会执行
 * @param - dev     : platform设备指针
 * @return          : 0，成功;其他负值,失败
 */
static int myled_remove(struct platform_device *dev)
{
    int i = 0;
    printk(KERN_INFO "myled: led platform driver remove!\r\n");
 
    /* 先关灯 */
    gpio_direction_output(myled[0].led_gpio, LED_OFF);
 
    /* 注销设备 */
    for ( i = 0; i < MYLED_CNT; i++)
    {
        device_destroy(myled_class, myled[i].devid);
    }
 
    /* 注销类 */
    class_destroy(myled_class);
 
    for ( i = 0; i < MYLED_CNT; i++)
    {
        /* 删除cdev */
        cdev_del(&myled[i].cdev);
 
        /* 注销设备号 */
//      unregister_chrdev_region(myled[i].devid, 1);    /* 固定的一个设备号一个设备的删除 0219lc */
    }
 
    unregister_chrdev_region(parent_devid, MYLED_CNT);  /* 经过验证，只释放父设备号就好了 0219lc */
 
    /* 删除地址映射 */
    gpio_free(myled[0].led_gpio);       /* 这里看一个设备树获取， 还是直接映射寄存器 0219lc */
 
    printk("remove module success! \r\n");
    return 0;
}
 
/* 匹配列表 */
static const struct of_device_id led_of_match[] = {
    { .compatible = "tasson,tasson_leds" },
    // { .compatible = "alientek,ps_led1" },        /* 这里 和 设备树节点的compatible值肯定是 唯一对应的，不然会报错 0219lc */
    { /* Sentinel */ }                          /* 这个地发驱动和失败树只能匹配一个，多个同时匹配 会报错 0219lc，那怎么操作2个设备呢 ？？ */
};                                              /* 并且显示加载了一个设备，卸载会出现段错误 */
 
/* platform驱动结构体 */
static struct platform_driver myled_driver = {
    .driver = {
        .name           = "zynq-led",       // 驱动名字，用于和设备匹配
        .of_match_table = led_of_match,     // 设备树匹配表，用于和设备树中定义的设备匹配
    },
    .probe      = myled_probe,  // probe函数
    .remove     = myled_remove, // remove函数
};
 
/*
 * @description     : 模块入口函数
 * @param           : 无
 * @return          : 无
 */
static int __init myled_driver_init(void)
{
    return platform_driver_register(&myled_driver);
}
 
/*
 * @description     : 模块出口函数
 * @param           : 无
 * @return          : 无
 */
static void __exit myled_driver_exit(void)
{
    platform_driver_unregister(&myled_driver);
}
 
module_init(myled_driver_init);
module_exit(myled_driver_exit);
 
MODULE_AUTHOR("LiuChuan <2858008125@qq.com>");
MODULE_DESCRIPTION("Led Platform Driver");
MODULE_LICENSE("GPL");