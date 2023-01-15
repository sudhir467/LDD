/* Basic character device driver - for demo*/
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>


/* If the return value is -1 you will not be able to insert the module into the kernel successfully*/
/*But you can see the message in the dmesg where it enters the init function*/
static int test_sudhir_borra_init(void)
{
    printk(KERN_INFO "%s : Module init \n",__func__);
    //return -1;
    return 0;
}

static void test_sudhir_borra_exit(void)
{
    printk(KERN_INFO "%s : Module exit\n",__func__);
}


module_init(test_sudhir_borra_init);
module_exit(test_sudhir_borra_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUDHIR_BORRA");
MODULE_DESCRIPTION("Basic Character Device Driver");