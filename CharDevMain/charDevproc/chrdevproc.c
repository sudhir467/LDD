/* Demonstrating character device driver using proc file system*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define mem_size 1024
char chr_array[40] = "Welcome to technogenius Academy\n";
static int len = 1;

// Define the ioctl code

#define WR_DATA _IOW('a', 'a', int32_t *)
#define RD_DATA _IOR('a', 'b', int32_t *)

int32_t val = 0;
dev_t dev = 0;

static struct class *dev_class;
static struct cdev my_cdev;
uint8_t *kernel_buffer;

static int __init chr_driver_init(void);
static void __exit chr_driver_exit(void);
static int chr_open(struct inode *inode, struct file *file);
static int chr_release(struct inode *inode, struct file *file);
static ssize_t chr_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t chr_write(struct file *filp, const char *buf, size_t len, loff_t *off);
static long chr_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static int open_proc(struct inode *inode, struct file *file);
static int release_proc(struct inode *inode, struct file *file);
static ssize_t read_proc(struct file *filp, char __user *buffer, size_t length, loff_t *offset);
static ssize_t write_proc(struct file *filp, const char *buff, size_t len, loff_t *off);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = chr_read,
    .write = chr_write,
    .open = chr_open,
    .unlocked_ioctl = chr_ioctl,
    .release = chr_release,
};

static struct file_operations proc_fops = {
    .open = open_proc,
    .write = write_proc,
    .read = read_proc,
    .release = release_proc,
};

static int open_proc(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Procfs file is opened.....\n");
    return 0;
}

static int release_proc(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "procfs file is released.....\n");
    return 0;
}

static ssize_t read_proc(struct file *filep, char __user *buffer, size_t length, loff_t *offset)
{
    printk(KERN_INFO "Procfs file reading....\n");
    if (len)
    {
        len = 0;
    }
    else
    {
        len = 1;
        return 0;
    }

    copy_to_user(buffer, chr_array, 40);
    return length;
}

static ssize_t write_proc(struct file *filep, const char *buff, size_t len, loff_t *off)
{
    printk(KERN_INFO "Procfs is writing....\n");
    copy_from_user(chr_array, buff, len);
    return len;
}

static int chr_open(struct inode *inode, struct file *file)
{
    /*creating physical memory*/
    if ((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0)
    {
        printk(KERN_INFO "Cannot allocate memory in kernel \n");
        return -1;
    }
    printk(KERN_INFO "Device file opened....!! \n");
    return 0;
}

static int chr_release(struct inode *inode, struct file *file)
{
    kfree(kernel_buffer);
    printk(KERN_INFO "Device file closed...\n");
    return 0;
}

static ssize_t chr_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    copy_to_user(buf, kernel_buffer, mem_size);
    printk(KERN_INFO "Data read: Done !! \n");
    return mem_size;
}

static ssize_t chr_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    copy_from_user(kernel_buffer, buf, len);
    printk(KERN_INFO "Data write: Done!!\n");
    return len;
}

static long chr_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
    case WR_DATA:
        copy_from_user(&val, (int32_t *)arg, sizeof(val));
        printk(KERN_INFO "val= %d \n", val);
        break;
    case RD_DATA:
        copy_to_user((int32_t *)arg, &val, sizeof(val));
        break;
    }
    return 0;
}

static int __init chr_driver_init(void)
{
    /* Allocating major number*/
    if ((alloc_chrdev_region(&dev, 0, 1, "my_Dev")) < 0)
    {
        printk(KERN_INFO "Cannot allocate the major number..\n");
        return -1;
    }

    printk(KERN_INFO "MAJOR=%d Minor= %d..\n", MAJOR(dev), MINOR(dev));

    /*creating cdev structure*/
    cdev_init(&my_cdev, &fops);

    /*Adding character device to the system*/
    if ((cdev_add(&my_cdev, dev, 1)) < 0)
    {
        printk(KERN_INFO "Cannot add the device to the system ...\n");
        goto r_class;
    }

    /*Creating struct class*/
    if ((dev_class = class_create(THIS_MODULE, "my_class")) == NULL)
    {
        printk(KERN_INFO "Cannot create the struct class...\n");
        goto r_class;
    }

    /* Creating device */
    if ((device_create(dev_class, NULL, dev, NULL, "my_device")) == NULL)
    {
        printk(KERN_INFO "Cannot create the device ..\n");
        goto r_device;
    }

    /* Creating proc entry*/
    /*proc_create(first parameter is name of the proc entry,mode,parent, file operations structure)*/

    proc_create("chr_proc", 0666, NULL, &proc_fops);

    printk(KERN_INFO "Device Driver insert ... done properly...\n");
    return 0;

r_device:
    class_destroy(dev_class);

r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

void __exit chr_driver_exit(void)
{
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device driver is removed successfully.. \n");
}

module_init(chr_driver_init);
module_exit(chr_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sudhir Borra");
MODULE_DESCRIPTION("Character device driver");
