/* Demonstration of character device drivers- Device file creation using static major minor number initialization*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/device.h>

dev_t dev1, dev2;
static struct class *class;
static struct device *device1, *device2;

static int test_hello_init(void)
{
    dev1 = MKDEV(200, 1);
    dev2 = MKDEV(200, 2);
    class = class_create(THIS_MODULE, "myclass");
    device1 = device_create(class, NULL, dev1, NULL, "mydevice%d", 1);
    device2 = device_create(class, NULL, dev2, NULL, "mydeive%d", 2);

    return 0;
}

static void test_hello_exit(void)
{
    device_destroy(class, dev1);
    device_destroy(class, dev2);
    class_destroy(class);
}

module_init(test_hello_init);
module_exit(test_hello_exit);

MODULE_LICENSE("GPL");