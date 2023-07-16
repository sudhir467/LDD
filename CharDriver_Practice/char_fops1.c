/* Demonstrating character device driver- file operations*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>

int base_minor = 0;
dev_t device_number;
char *device_name = "mychardev";
int count = 1;

static struct class *class = NULL;
static struct device *device = NULL;
static struct cdev mycdev;

static int device_open(struct inode *inode, struct file *file)
{
    pr_info("%s\n", __func__);
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    pr_info("%s \n", __func__);
    return 0;
}

static ssize_t device_read(struct file *file, char __user *user_buffer, size_t count, loff_t *offset)
{
    pr_info("%s \n", __func__);
    return 0;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *offset)
{
    pr_info("%s \n", __func__);
    return count;
}

struct file_operations device_fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
    .owner = THIS_MODULE
};

static int test_hello_init(void)
{
    class = class_create(THIS_MODULE, "myclass");
    if ((alloc_chrdev_region(&device_number, base_minor, count, device_name))<0)
        {
            printk("Device number registration failed \n");
        }
    else
    {
        printk("Device_number registered \n");
        printk("Major number recieved: %d \n", MAJOR(device_number));
        device = device_create(class, NULL, device_number, NULL, device_name);
        cdev_init(&mycdev, &device_fops);
        cdev_add(&mycdev, device_number, count);
    }

    return 0;
}

static void test_hello_exit(void)
{
    device_destroy(class, device_number);
    class_destroy(class);
    cdev_del(&mycdev);
    unregister_chrdev_region(device_number, count);
}

module_init(test_hello_init);
module_exit(test_hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUDHIR_BORRA");