/*Demonstration of character device drivers- Creating Device file- Creating a struct class*/

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>

static struct class *class;

static int test_hello_init(void)
{
    class= class_create(THIS_MODULE, "my_class");
    return 0;
}

static void test_hello_exit(void)
{
    class_destroy(class);
}

module_init(test_hello_init);
module_exit(test_hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUDHIR BORRA");
MODULE_DESCRIPTION("CREATING DEVICE FILE");
