/*Demonstation of character device driver - Dynamic allocation of Major and minor number */

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>

int base_minor =0;
char *device_name="mychardev";
int count =1;
dev_t devicenumber;

module_param(base_minor,int,0);
module_param(count,int,0);
module_param(device_name,charp,0);

static int test_hello_init(void)
{
    printk("Minor number: %d \n",base_minor);
    printk("count: %d \n",count);
    printk("Device name: %s \n", device_name);

    if(! alloc_chrdev_region(&devicenumber, base_minor,count,device_name))
    {
        printk("Device number registered \n");
        printk("Major number recieved:%d \n", MAJOR(devicenumber));
    }
    else
    {
        printk("Device number registration failed \n");
    }

    return 0;
}

static void test_hello_exit(void)
{
    unregister_chrdev_region(devicenumber, count);
}

module_init(test_hello_init);
module_exit(test_hello_exit);

MODULE_AUTHOR("SUDHIR BORRA");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("DYNAMIC ALLOCATION OF MAJOR AND MINOR NUMBER");