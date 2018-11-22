#include<linux/init.h>
#include<linux/module.h>                                        
#include "mach/../../mx28_pins.h"
// #include <arch/arm/mx28_pins.h>
#include <linux/leds.h>

#define LED_GPIO        MXS_PIN_TO_GPIO(PINID_LCD_D23)  /*ERR LED的GPIO*/

static void mxs_led_brightness_set(struct led_classdev *pled,enum led_brightness value)
{
        gpio_direction_output(LED_GPIO,!value);
}

struct led_classdev led_dev = {
        .name           ="led-example",                 /*设备名称为led-example*/
        .brightness_set =mxs_led_brightness_set,
        .default_trigger="none",                        /*默认使用none触发器*/
};

static int __init led_init(void)
{
        int ret = 0;

        ret = led_classdev_register(NULL,&led_dev);     /*注册LED设备*/
        if(ret){
                printk("register led device faile \n");
                return -1;
        }
        gpio_request(LED_GPIO,"led");                   /*申请GPIO*/
        return 0;
}

static void __exit led_exit(void)
{
        led_classdev_unregister(&led_dev);              /*注销LED设备*/
        gpio_free(LED_GPIO);                            /*释放GPIO*/
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");