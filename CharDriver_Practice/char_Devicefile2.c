/* Demonstrating character device driver- Device file creation*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/fs.h>

int count = 1;
char *device_name = "mychardevice";
char base_minor = 25;
dev_t device_number;

static struct class *class;
static struct device *device;

static int test_hello_init(void)
{

    class = class_create(THIS_MODULE, "my_class");

    if ((alloc_chrdev_region(&device_number, base_minor, count, device_name)) < 0)
    {
        printk("Device number registration failed\n");
    }
    else
    {
        printk("Device number registerd \n");
        printk("Major number returned: (%d) Minor number: (%d) \n", MAJOR(device_number), MINOR(device_number));
        device = device_create(class, NULL, device_number, NULL, device_name);
    }

    return 0;
}

static void test_hello_exit(void)
{
    unregister_chrdev_region(device_number, count);
    device_destroy(class, device_number);
    class_destroy(class);
}

module_init(test_hello_init);
module_exit(test_hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUDHIR");
MODULE_DESCRIPTION("DEVICE FILE CREATION");
