/*
* @Author: bingo
* @Date:   2022-08-24 23:30:31
* @Last Modified by:   bingo
* @Last Modified time: 2022-08-30 23:43:43
*/
#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/err.h>

#define MYLED_NAME      "myled"     /* 名字 */

#define LED_ON          0
#define LED_OFF         1

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

static struct myled_dev *priv;
static int count;

/*
 * @description     : 打开设备
 * @param – inode   : 传递给驱动的inode的成员变量i_rdev为设备编号
 * @param - filp    : 设备文件，file结构体有个叫做private_data的成员变量
 *                    一般在open的时候将private_data指向设备结构体。
 * @return          : 0 成功;其他 失败
 */
static int myled_open(struct inode *inode, struct file *filp)
{
    int i;

    for (i = 0; i < count; ++i)
    {
        if(priv[i].devid == inode->i_rdev) /*匹配设备编号*/
        {
            filp->private_data = (void *)&priv[i];	/*将设备数据传递给private_data*/
            return 0;
        }
    }

    printk("open can't find dev\n");
    return -1;
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
    struct myled_dev *write_dev = filp->private_data;

    ret = copy_from_user(kern_buf, buf, cnt);   // 得到应用层传递过来的数据
    if (0 > ret) {
        printk(KERN_ERR "myled: Failed to copy data from user buffer\r\n");
        return -EFAULT;
    }
    // printk("state=%d\n",kern_buf[0]);
    printk("led_gpio = %d state=%d\n",write_dev->led_gpio,kern_buf[0]);
    if (0 == kern_buf[0])
     gpio_set_value(write_dev->led_gpio, 0);      // 如果传递过来的数据是0则关闭led
    else if (1 == kern_buf[0])
     gpio_set_value(write_dev->led_gpio, 1);      // 如果传递过来的数据是1则点亮led

    return 0;
}

static int myled_init(struct myled_dev *myled_priv)
{
    const char *str;
    int val;
    int ret;

    if (myled_priv == NULL)
    {
        printk("myled_priv is NULL\n");
        return 0;
    }

    /* 从设备树中获取GPIO */
    myled_priv->led_gpio = of_get_named_gpio(myled_priv->nd, "led-gpio", 0);       /* 获取gpio num */
    if (!gpio_is_valid(myled_priv->led_gpio)) {
        printk(KERN_ERR "myled: Failed to get led-gpio %d\n", myled_priv->led_gpio);
        return -EINVAL;
    }

    /* 申请使用GPIO */
    ret = gpio_request(myled_priv->led_gpio, "PS_LED0 Gpio");
    if (ret) {
        printk(KERN_ERR "myled: Failed to request led-gpio\n");
        return ret;
    }

    /* 确定LED初始状态 */
    ret = of_property_read_string(myled_priv->nd, "default-state", &str);
    if (!ret) {
        if (!strcmp(str, "on"))
            val = 1;
        else
            val = 0;
    } else
        val = 0;

    /* 将GPIO设置为输出模式并设置GPIO初始电平状态 */
    gpio_direction_output(myled_priv->led_gpio, val);

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
    struct fwnode_handle *child;
    struct device *dev = &pdev->dev;

    count = device_get_child_node_count(dev);	/* 获取设备数量  */
    if (!count)
        return -1;

    printk(KERN_INFO "myled: led driver and device has matched! device num=%d\r\n", count);
    priv = devm_kzalloc(dev, sizeof(struct myled_dev) + (sizeof(struct myled_dev) * count), GFP_KERNEL);	/* 动态分配空间  */
    if (!priv)
    {
        printk("devm_kzalloc error\n");
        return -1;
    }

    parent_nd = of_find_node_by_path("/tasson_leds"); /* 通过名字寻找大节点 */
    if (NULL == parent_nd)
    {
        printk("parent_nd error\n");
        return -1;
    }

    i = 0;
    device_for_each_child_node(dev, child) /* 遍历所有节点提取信息 */
    {
        priv[i].nd = to_of_node(child);
        ret = myled_init(&priv[i]);   
        if (ret)
        {
            printk("myled init fail i=%d\n", i);
            return ret;
        }
        i++;
    }

    /* 申请父设备号，（为了创建子设备号） */
    ret = alloc_chrdev_region(&parent_devid, 0, count, MYLED_NAME);     /* 该名字在 cat /proc/devices 下面可看到设备号 */
    if (ret)
        printk("alloc_chrdev_region fail ! \r\n");
//      goto out1;
    major_devid = MAJOR(parent_devid);
    printk("request major_devid is %d \r\n", major_devid);

    /* 给每个设备设置设备号，添加内核， */
    for ( i = 0; i < count; i++)
    {
        /* 设置次设备号 */
        priv[i].devid = parent_devid + i;

        /* 初始化cdev */
        priv[i].cdev.owner = THIS_MODULE;
        cdev_init(&priv[i].cdev, &myled_fops);     /* 每个设备都是绑定同一个 fops 操作函数结构体  */

        /* 添加cdev ，设备添加到内核 */
        ret = cdev_add(&priv[i].cdev, priv[i].devid, 1);  /* 此处是固定的一个一个添加  */
        if (ret)
            goto out2;
    }

    /* 创建类class ，无论是一个设备还是多个设备类只有一个  */
    myled_class = class_create(THIS_MODULE, MYLED_NAME);
    if (IS_ERR(myled_class)) {
        ret = PTR_ERR(myled_class);
        goto out3;
    }

    /* 创建设备 */
    for ( i = 0; i < count; i++)
    {
        priv[i].device = device_create(myled_class, &pdev->dev,
                                       priv[i].devid, NULL, "%s%d", MYLED_NAME, i);       /* 给设备组成名字的方式  */
        if (IS_ERR(priv[i].device)) {
            ret = PTR_ERR(priv[i].device);
            goto out4;
        }
    }

    platform_set_drvdata(pdev, priv);
    printk("load module success! \r\n"); 

    return 0;

out4:
    class_destroy(myled_class);

out3:
    for ( i = 0; i < count; i++)
    {
        cdev_del(&priv[i].cdev);
    }

out2:
    unregister_chrdev_region(parent_devid, count);  /* 把申请的设备号都注销掉  */

// out1:
//  gpio_free(myled.led_gpio);  /* 申请使用几个gpio引脚 ？ 设备是一个使用，同时使用  */

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
    struct myled_dev *priv = platform_get_drvdata(dev);

    printk(KERN_INFO "myled: led platform driver remove!\r\n");

    /* 先关灯 */
    for (i = 0; i < count; i++)
    {
        gpio_direction_output(priv[i].led_gpio, LED_OFF);
        gpio_free(priv[i].led_gpio);

        cdev_del(&priv[i].cdev);/* 删除cdev */
        device_destroy(myled_class, priv[i].devid);/* 注销设备 */
    }

    unregister_chrdev_region(parent_devid, count);  /* 释放父设备号，及对应的次设备号数量  */

    /* 注销类 */
    class_destroy(myled_class);

    printk("remove module success! \r\n");
    return 0;
}

/* 匹配列表 */
static const struct of_device_id led_of_match[] = {
    { .compatible = "tasson,tasson_leds" },
    // { .compatible = "alientek,ps_led1" },        /* 这里 和 设备树节点的compatible值肯定是 唯一对应的，不然会报错  */
    { /* Sentinel */ }                          /* 这个地发驱动和失败树只能匹配一个，多个同时匹配 会报错 ，那怎么操作2个设备呢 ？？ */
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

MODULE_AUTHOR("bingo@qq.com");
MODULE_DESCRIPTION("Led Platform Driver");
MODULE_LICENSE("GPL");
