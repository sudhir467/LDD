/* Demonstration to allocate major and minor number to character driver*/

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/kdev_t.h>

dev_t devicenumber;

static int test_hello_init(void)
{
    printk("Major number:%d \n", MAJOR(devicenumber));
    printk("Minor number:%d \n", MINOR(devicenumber));

    devicenumber = 49;
    printk("Major number:%d \n", MAJOR(devicenumber));
    printk("Minor number:%d \n", MINOR(devicenumber));

    devicenumber=MKDEV(120, 30);
    printk("Major number:%d \n", MAJOR(devicenumber));
    printk("Minor number:%d \n", MINOR(devicenumber));

    return 0;
}

static void test_hello_exit(void)
{
    printk(KERN_INFO"%s : In exit() \n", __func__);
}

module_init(test_hello_init);
module_exit(test_hello_exit);

MODULE_AUTHOR("SUDHIR BORRA");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MAJOR MINOR NUMBER");
