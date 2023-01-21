/*Demonstrating character device driver- Copying from kernel space to user space
---------------------------copy_to_user---------------------------------------*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

int baseminor = 0;
dev_t devicenumber;
char *device_name = "mychardev";
int count = 1;

static struct class *class;
static struct device *device;
static struct cdev mycdev;

static int mydevice_open(struct inode *inode, struct file *file)
{
    pr_info("%s:Device opened success\n", __func__);
    return 0;
}

static int mydevice_release(struct inode *inode, struct file *file)
{
    pr_info("%s: Device released \n", __func__);
    return 0;
}

static ssize_t mydevice_read(struct file *file, char __user *userbuffer, size_t count, loff_t *offset)
{
    char kernel_buffer[10] = "kernel";
    int retval;
    retval = copy_to_user(userbuffer, kernel_buffer, 7);
    pr_info("%s:copy_to_user returned:%d \n", __func__, retval);
    pr_info("%s: Kernel buffer:%p \t count:%lu \t offset:%llu \n", __func__, kernel_buffer, count, *offset);
    return count;
}

static ssize_t mydevice_write(struct file *file, const char __user *userbuffer, size_t count, loff_t *offset)
{
    char kernel_buffer[100] = {0};
    int retval;
    retval = copy_from_user(kernel_buffer, userbuffer, count);
    pr_info("%s: copy_from_user returned:%d \n", __func__, retval);
    pr_info("%s: Kernel_buffer: %p \t count:%lu \t offset:%llu \n", __func__, kernel_buffer, count, *offset);
    return count;
}

static struct file_operations my_fops = {
    .read = mydevice_read,
    .write = mydevice_write,
    .open = mydevice_open,
    .release = mydevice_release,
    .owner = THIS_MODULE};

static int test_hello_init(void)
{
    class = class_create(THIS_MODULE, "myclass");
    if (!alloc_chrdev_region(&devicenumber, baseminor, count, device_name))
    {
        printk(" Device number registerd !! \n");
        printk(" Major number returned:%d\n", MAJOR(devicenumber));
        device = device_create(class, NULL, devicenumber, NULL, "mydevice");
        cdev_init(&mycdev, &my_fops);
        cdev_add(&mycdev, devicenumber, count);
    }
    else
    {
        printk("Device number registration failed \n");
    }
    return 0;
}

static void test_hello_exit(void)
{
    device_destroy(class,devicenumber);
    class_destroy(class);
    cdev_del(&mycdev);
    unregister_chrdev_region(devicenumber,count);
}

module_init(test_hello_init);
module_exit(test_hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUDHIR_BORRA");
