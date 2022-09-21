/*
* @Author: bingo
* @Date:   2022-08-22 23:46:42
* @Last Modified by:   bingo
* @Last Modified time: 2022-09-21 23:33:25
*/

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of_gpio.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/of_address.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "ks103.h"

#define KS103_CNT 1
#define KS103_NAME "ks103"

struct ks103_dev {
    struct cdev cdev; /* cdev */
    dev_t devid; /* 设备号 */
    struct class *class; /* 类 */
    struct device *device; /* 设备 */
    struct device_node *nd; /* 设备节点 */
    struct i2c_client *client;
    struct device_node *node;/*设备树节点*/
    int major; /* 主设备号 */
    void *private_data; /* 私有数据 */
    unsigned short temp, light, distance; /* 三个光传感器数据 */
};

static struct class *mydev_class;  /* 类 */
static int count = 0;
static dev_t parent_devid;         /* 设备号 */

/*
* @description : 从 ks103 读取多个寄存器数据
* @param – dev : ks103 设备
* @param – reg : 要读取的寄存器首地址
* @param – val : 读取到的数据
* @param – len : 要读取的数据长度
* @return : 操作结果
*/
static int ks103_read_regs(struct ks103_dev *dev, u8 reg, void *val, int len)
{
    int ret;
    struct i2c_msg msg[2];
    struct i2c_client *client = (struct i2c_client *)dev->client;
    /* msg[0]为发送要读取的首地址 */
    msg[0].addr = client->addr; /* ks103 地址 */
    msg[0].flags = 0; /* 标记为发送数据 */
    msg[0].buf = &reg; /* 读取的首地址 */
    msg[0].len = 1; /* reg 长度 */

    /* msg[1]读取数据 */
    msg[1].addr = client->addr; /* ks103 地址 */
    msg[1].flags = I2C_M_RD; /* 标记为读取数据 */
    msg[1].buf = val; /* 读取数据缓冲区 */
    msg[1].len = len; /* 要读取的数据长度 */

    ret = i2c_transfer(client->adapter, msg, 2);
    if (ret == 2) {
        ret = 0;
    } else {
        printk("i2c rd failed=%d reg=%06x len=%d\n", ret, reg, len);
        ret = -EREMOTEIO;
    }
    return ret;
}

/*
* @description : 向 ks103 多个寄存器写入数据
* @param – dev : ks103 设备
* @param – reg : 要写入的寄存器首地址
* @param – val : 要写入的数据缓冲区
* @param – len : 要写入的数据长度
* @return : 操作结果
*/
static s32 ks103_write_regs(struct ks103_dev *dev, u8 reg, u8 *buf, u8 len)
{
    u8 b[256];
    struct i2c_msg msg;
    struct i2c_client *client = (struct i2c_client *)dev->client;

    b[0] = reg; /* 寄存器首地址 */
    memcpy(&b[1], buf, len); /* 将要写入的数据拷贝到数组 b 里面 */

    msg.addr = client->addr; /* ks103 地址 */
    msg.flags = 0; /* 标记为写数据 */

    msg.buf = b; /* 要写入的数据缓冲区 */
    msg.len = len + 1; /* 要写入的数据长度 */

    return i2c_transfer(client->adapter, &msg, 1);
}

/*
* @description : 读取 ks103 指定寄存器值，读取一个寄存器
* @param – dev : ks103 设备
* @param – reg : 要读取的寄存器
* @return : 读取到的寄存器值
*/
/*static unsigned char ks103_read_reg(struct ks103_dev *dev, u8 reg)
{
    u8 data = 0;

    ks103_read_regs(dev, reg, &data, 1);
    return data;

#if 0
    struct i2c_client *client = (struct i2c_client *)dev->client;
    return i2c_smbus_read_byte_data(client, reg);
#endif
}*/

/*
* @description : 向 ks103 指定寄存器写入指定的值，写一个寄存器
* @param – dev : ks103 设备
* @param – reg : 要写的寄存器
* @param – data : 要写入的值
* @return : 无
*/
static void ks103_write_reg(struct ks103_dev *dev, u8 reg, u8 data)
{
    u8 buf = 0;
    buf = data;
    ks103_write_regs(dev, reg, &buf, 1);
}

/*
* @description : 读取 ks103 的数据，读取原始数据，包括 light,distance 和 temp,
* :同时打开 light,temp+distance 的话两次数据读取的间隔要大于 112.5ms
* @param - temp : temp 数据
* @param - distance : distance 数据
* @param - distance : light 数据
* @return : 无。
*/
void ks103_readdata(struct ks103_dev *dev)
{
    unsigned char buf[6];

    /* 循环读取所有传感器数据 */
    ks103_write_reg(dev, 0x02, 0xa0);
    mdelay(3);
    ks103_read_regs(dev, 0x02, buf, 2);
    dev->light = (buf[0] << 8) + buf[1];

    ks103_write_reg(dev, 0x02, 0xcc);
    mdelay(610);
    ks103_read_regs(dev, 0x02, buf, 2);
    dev->temp = (buf[0] << 8) + buf[1];

    ks103_write_reg(dev, 0x02, 0xb2);
    mdelay(33);
    ks103_read_regs(dev, 0x02, buf, 2);
    dev->distance = (buf[0] << 8) + buf[1];

    // dev->temp = 0;
    // dev->distance = 0;
}

/*
* @description : 打开设备
* @param – inode : 传递给驱动的 inode
* @param - filp : 设备文件，file 结构体有个叫做 private_data 的成员变量
* 一般在 open 的时候将 private_data 指向设备结构体。
* @return : 0 成功;其他 失败
*/
static int ks103_open(struct inode *inode, struct file *filp)
{
    struct ks103_dev *dev = container_of(inode->i_cdev, struct ks103_dev, cdev);
    filp->private_data = dev;

    // printk("%s addr=%x %x\n", __FUNCTION__,dev,dev->client->addr);

    return 0;
}

/*
* @description : 从设备读取数据
* @param – filp : 要打开的设备文件(文件描述符)
* @param - buf : 返回给用户空间的数据缓冲区
* @param - cnt : 要读取的数据长度
* @param – offt : 相对于文件首地址的偏移
* @return : 读取的字节数，如果为负值，表示读取失败
*/
static ssize_t ks103_read(struct file *filp, char __user *buf, size_t cnt, loff_t *off)
{
    short data[3];
    long err = 0;

    struct ks103_dev *dev = (struct ks103_dev *)filp->private_data;

    ks103_readdata(dev);

    data[0] = dev->temp;
    data[1] = dev->light;
    data[2] = dev->distance;
    err = copy_to_user(buf, data, sizeof(data));
    return 0;
}

/*
* @description : 关闭/释放设备
* @param - filp : 要关闭的设备文件(文件描述符)
* @return : 0 成功;其他 失败
*/
static int ks103_release(struct inode *inode, struct file *filp)
{
    return 0;
}

/* ks103 操作函数 */
static const struct file_operations ks103_ops = {
    .owner = THIS_MODULE,
    .open = ks103_open,
    .read = ks103_read,
    .release = ks103_release,
};

/*
* @description : i2c 驱动的 probe 函数，当驱动与
* 设备匹配以后此函数就会执行
* @param - client : i2c 设备
* @param - id : i2c 设备 ID
* @return : 0，成功;其他负值,失败
*/
static int ks103_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int ret;
    struct ks103_dev *ks103_dev_data;
    struct device *dev = &client->dev;

    ks103_dev_data = devm_kzalloc(dev, sizeof(*ks103_dev_data), GFP_KERNEL);
    if (!ks103_dev_data)
        return -ENOMEM;

    if ( 0 == count)
    {
        alloc_chrdev_region(&parent_devid, 0, 1, KS103_NAME);
        printk("request major_devid is %d \r\n", MAJOR(parent_devid));
    }

    /*printk("client->name=%s\n",client->name);
    printk("client->adapter->name=%s\n",client->adapter->name);
    printk("client->dev.of_node->name=%s\n",client->dev.of_node->name);
    for_each_compatible_node(ks103_dev_data->node, NULL, KS103_NAME)//获取节点名字
    {
        const __be32 *addr_dtc;

        addr_dtc = of_get_address(ks103_dev_data->node, 0, NULL, NULL);
        if (be32_to_cpup(addr_dtc) == client->addr)
        {
            printk("name=%s bus=%d addr_dtc=%x\n",ks103_dev_data->node->name,client->flags,be32_to_cpup(addr_dtc));
            break;
        }
    }*/

    /* 2、注册设备 */
    ks103_dev_data->devid = parent_devid + count;
    ks103_dev_data->cdev.owner = THIS_MODULE;
    cdev_init(&ks103_dev_data->cdev, &ks103_ops);
    ret = cdev_add(&ks103_dev_data->cdev, ks103_dev_data->devid, 1);
    if (ret)
    {
        printk("cdev_add failed\n");
    }

    /* 3、创建类 */
    if ( 0 == count )
    {
        mydev_class = class_create(THIS_MODULE, KS103_NAME);
        if (IS_ERR(mydev_class)) {
            return PTR_ERR(mydev_class);
        }
    }
    /* 4、创建设备 */
    ks103_dev_data->device = device_create(mydev_class, &client->dev,
                                           ks103_dev_data->devid, NULL, 
                                           "%s", client->dev.of_node->name);
    if (IS_ERR(ks103_dev_data->device)) {
        return PTR_ERR(ks103_dev_data->device);
    }

    ks103_dev_data->client = client;
    client->dev.driver_data = (void *)ks103_dev_data;

    // printk("addr client=%x data=%x\n",client,ks103_dev_data);

    count += 1;

    return 0;
}

/*
* @description : i2c 驱动的 remove 函数，移除 i2c 驱动此函数会执行
* @param – client : i2c 设备
* @return : 0，成功;其他负值,失败
*/
static int ks103_remove(struct i2c_client *client)
{
    struct ks103_dev *ks103_dev_data = client->dev.driver_data;

    // printk("addr client=%x data=%x\n",client,client->dev.driver_data);
    /* 删除设备 */
    cdev_del(&ks103_dev_data->cdev);
    device_destroy(mydev_class, ks103_dev_data->devid);
    unregister_chrdev_region(ks103_dev_data->devid, 1);
    count--;

    /* 注销掉类和设备 */
    if (count == 0)
    {
        class_destroy(mydev_class);
    }
    return 0;
}

/* 传统匹配方式 ID 列表 */
static const struct i2c_device_id ks103_id[] = {
    {"ks103", 0},
    {}
};

/* 设备树匹配列表 */
static const struct of_device_id ks103_of_match[] = {
    { .compatible = "ks103" },
    { .compatible = "ks103_88" },
    { /* Sentinel */ }
};

/* i2c 驱动结构体 */
static struct i2c_driver ks103_driver = {
    .probe = ks103_probe,
    .remove = ks103_remove,
    .driver = {
        .owner = THIS_MODULE,
        .name = "ks103_1",
        .of_match_table = ks103_of_match,
    },
    .id_table = ks103_id,
};

/*
* @description : 驱动入口函数
* @param : 无
* @return : 无
*/
static int __init ks103_init(void)
{
    int ret = 0;

    ret = i2c_add_driver(&ks103_driver);
    return ret;
}

/*
* @description : 驱动出口函数
* @param : 无
* @return : 无
*/
static void __exit ks103_exit(void)
{
    i2c_del_driver(&ks103_driver);
}

/* module_i2c_driver(ks103_driver) */

module_init(ks103_init);
module_exit(ks103_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("bingo");