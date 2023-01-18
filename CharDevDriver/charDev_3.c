/* Basic Character Device driver- for demo */

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/delay.h>

static int hello_world_init(void)
{
    printk(KERN_INFO "%s : In Init \n", __func__);
    return 0;
}

static void hello_world_exit(void)
{
    printk(KERN_INFO "%s: In exit \n", __func__);
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sudhir Borra");
MODULE_DESCRIPTION("Basic Character device driver");