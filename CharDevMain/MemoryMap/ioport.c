/*This module is to request a particular region of memory */

#include<linux/module.h>
#include<linux/ioport.h>
#include<linux/io.h>
#include<linux/init.h>


static char __iomem *mapped;

#define IOSTART 0x200
#define IOEXTEND 0x40

static unsigned long iostart = IOSTART,ioextend =IOEXTEND,ioend;

static int __init my_init(void)
{
	unsigned long ultest = (unsigned long)100;

	ioend = iostart +ioextend;

	printk(KERN_INFO"requesting the IO region 0x%lx to 0x%lx\n",iostart,ioend);

	if(!request_region(iostart,ioextend,"my_ioport")) {
		printk(KERN_INFO"the IO region is busy, quitting..\n");
		return -EBUSY;
	}

	mapped = ioport_map(iostart,ioextend);

	printk(KERN_INFO"ioport mapped at %p\n",mapped);
	printk(KERN_INFO"Writing a data =%ld\n",ultest);
	
	iowrite32(ultest,mapped);

	ultest = ioread32(mapped);

	
	printk(KERN_INFO"Reading a data =%ld\n",ultest);


	return 0;

}


static void __exit my_exit(void)
{
	printk(KERN_INFO"releasing the IO region 0x%lx to 0x%lx\n",iostart,ioend);

	release_region(iostart,ioextend);
	ioport_unmap(mapped);

}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("TechoGenius Academy");
MODULE_DESCRIPTION(" requesting the I/O Ports");
MODULE_LICENSE("GPL");