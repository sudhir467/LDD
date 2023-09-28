/*Demonstrating Character device driver on how to copy data from user space to kernel space
--------------------------copy_from_user-----------------------------------------------*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

int baseminor = 0;
char *device_name = "mychardev";
int count = 1;
dev_t devicenumber;

static struct class *class = NULL;
static struct device *device = NULL;
static struct cdev mycdev;

static int device_open(struct inode *inode, struct file *file)
{
    pr_info("%s \n", __func__);
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    pr_info("%s \n", __func__);
    return 0;
}

static ssize_t device_read(struct file *file, char __user *userbuffer, size_t count, loff_t *offset)
{
    pr_info("%s:count:%lu \t offset:%llu \n", __func__, count, *offset);
    return 0;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *offset)
{
    char kernel_buffer[100] = {0};
    int retval;
    pr_info("%s: Kernel buffer:%p \t user buffer:%p \n", __func__, kernel_buffer, user_buffer);
    retval = copy_from_user(kernel_buffer, user_buffer, count);
    pr_info("%s: copy from user returned:%d \n", __func__, retval);
    pr_info("%s: Kernel buffer:%s \t count:%lu \t offset:%llu \n", __func__, kernel_buffer, count, *offset);
    return count;
}

struct file_operations device_fops = {
    .read = device_read,
    .write = device_write,
    .owner = THIS_MODULE,
    .release = device_release,
    .open = device_open};

static int test_hello_init(void)
{
    class = class_create(THIS_MODULE, "myclass");
    if (!alloc_chrdev_region(&devicenumber, baseminor, count, device_name))
    {
        printk("Device number registered \n");
        printk("Major number: %d \n", MAJOR(devicenumber));
        device = device_create(class, NULL, devicenumber, NULL, "mydevice");
        cdev_init(&mycdev, &device_fops);
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
    device_destroy(class, devicenumber);
    class_destroy(class);
    cdev_del(&mycdev);
    unregister_chrdev_region(devicenumber, count);
}

module_init(test_hello_init);
module_exit(test_hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUDHIR_BORRA");
