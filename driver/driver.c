/*  blink.c - The simplest kernel module.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include <linux/fs.h>

#include <linux/slab.h>
#include <linux/io.h>
#include <linux/interrupt.h>

#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>

#include <asm/uaccess.h> /* for get_user and put_user */
#include <asm/io.h>
// #include "blink.h"
#include "address.h"
#define SUCCESS 0
#define DEVICE_NAME "/dev/Sha1_module"

#define SHA1_BASEADDR 0x43C00000
#define SHA1_0 0
#define SHA1_1 1
#define SHA1_2 2
#define SHA1_3 3
#define SHA1_4 4
#define SHA1_5 5
#define SHA1_6 6
#define SHA1_7 7
#define SHA1_8 8
#define SHA1_9 9
#define SHA1_10 10
#define SHA1_11 11
#define SHA1_12 12
#define SHA1_13 13
#define SHA1_14 14
#define SHA1_15 16
#define SHA1_16 16
#define SHA1_17 17
#define SHA1_18 18
#define SHA1_19 19
#define SHA1_20 20
#define SHA1_21 21
#define SHA1_22 22
#define SHA1_23 23
#define SHA1_24 24
#define SHA1_25 25
#define SHA1_26 26
#define SHA1_27 27

// #define BLINK_CTRL_REG 0x43C00000
static unsigned long *mmio;
static unsigned long *selected_register;
static int major_num;

/*
 * Is the device open right now? Used to prevent
 * concurent access into the same device
 */
static int Device_Open = 0;

/*
static void set_blink_ctrl(void)
{
	printk("KERNEL PRINT : set_blink_ctrl \n\r");
	*(unsigned int *)mmio = 0x1;
}

static void reset_blink_ctrl(void)
{

	printk("KERNEL PRINT : reset_blink_ctrl \n\r");
	*(unsigned int *)mmio = 0x0;
}
*/
/*
 * This is called whenever a process attempts to open the device file
 */
static int device_open(struct inode *inode, struct file *file)
{
#ifdef DEBUG
	printk(KERN_INFO "device_open(%p)\n", file);
#endif
	/*
	 * We don't want to talk to two processes at the same time
	 */
	if (Device_Open)
		return -EBUSY;
	Device_Open++;

	selected_register = (unsigned long *)(mmio + SHA1_0);
	*selected_register = 0x4;
	*selected_register = 0x0;

	selected_register = (unsigned long *)(mmio + SHA1_21);
	*selected_register = 0x67452301;

	selected_register = (unsigned long *)(mmio + SHA1_20);
	*selected_register = 0xEFCDAB89;

	selected_register = (unsigned long *)(mmio + SHA1_19);
	*selected_register = 0x98BADCFE;

	selected_register = (unsigned long *)(mmio + SHA1_18);
	*selected_register = 0x10325476;

	selected_register = (unsigned long *)(mmio + SHA1_17);
	*selected_register = 0xC3D2E1F0;
	/*
	 * Initialize the message
	 */
	//	Message_Ptr = Message;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}
static int device_release(struct inode *inode, struct file *file)
{
#ifdef DEBUG
	printk(KERN_INFO "device_release(%p,%p)\n", inode, file);
#endif
	/*
	 * We're now ready for our next caller
	 */
	Device_Open--;
	
	selected_register = (unsigned long *)(mmio + SHA1_0);
	*selected_register = 0x4;
	*selected_register = 0x0;
	
	module_put(THIS_MODULE);
	return SUCCESS;
}
/*
 * This function is called whenever a process which has already opened the
 * device file attempts to read from it.
 */
static ssize_t device_read(struct file *file,	/* see include/linux/fs.h */
						   char __user *buffer, /* buffer to be filled with data */
						   size_t length,		/* length of the buffer */
						   loff_t *offset)
{
	ssize_t bytes = 0;
	static unsigned long *func_register;
	
	func_register = selected_register;
	
    while(length != 0) {
		printk(KERN_INFO "Sha1: Executing READ: selected register %lu, value %x\n", func_register, *func_register);
        // NOTE: Post increment unary operator must be used, copy_to_user returns 0 if OK
        if(copy_to_user(buffer, func_register, 4) == 0){
            // Decresce the lenght and increment the bytes counter
			buffer = buffer + 1;
			func_register = func_register + 1;
            length = length - 4;
            bytes = bytes + 4;
        } else{
            break;
        }
    }

    //printk(KERN_INFO "Sha1: Executing READ\n");

    return bytes;
}
/*
 * This function is called when somebody tries to
 * write into our device file.
 */
static ssize_t device_write(struct file *file,
							const char __user *buffer,
							size_t length,
							loff_t *offset)
{
	ssize_t bytes = 0;
	int tmp;
	static unsigned long *func_register;
	
	func_register = selected_register;
	
    while(length != 0) {
		copy_from_user(&tmp, buffer, 4);
		printk(KERN_INFO "Sha1: Executing WRITE: selected register %lu, value %x\n", func_register, tmp);
        // NOTE: Post increment unary operator must be used, copy_to_user returns 0 if OK
        if(copy_from_user(func_register, buffer, 4) == 0){
            // Decresce the lenght and increment the bytes counter
			func_register = func_register + 1;
			buffer = buffer + 1;
            length = length - 4;
            bytes = bytes + 4;
        } else{
            break;
        }
    }
    
   	//printk(KERN_INFO "Sha1: Executing WRITE\n");

	return bytes;
}
/*
 * This function is called whenever a process tries to do an ioctl on our
 * device file. We get two extra parameters (additional to the inode and file
 * structures, which all device functions get): the number of the ioctl called
 * and the parameter given to the ioctl function.
 *
 * If the ioctl is write or read/write (meaning output is returned to the
 * calling process), the ioctl call returns the output of this function.
 *
 */
long device_ioctl(struct file *file,	  /* ditto */
				  unsigned int ioctl_num, /* number and param for ioctl */
				  unsigned long ioctl_param)
{
	//	int i;
	char *temp;
	//	char ch;
	/*
	 * Switch according to the ioctl called
	 */
	switch (ioctl_num)
	{
	case DIN:
		selected_register = (unsigned long *)(mmio + SHA1_1);
		break;
	case CV:
		selected_register = (unsigned long *)(mmio + SHA1_17);
		break;
	case PREV_CV:
		selected_register = (unsigned long *)(mmio + SHA1_0);
		break;
	case START:
		selected_register = (unsigned long *)(mmio + SHA1_0);
		break;
	case BUSY:
		selected_register = (unsigned long *)(mmio + SHA1_22);
		break;
	case VALID:
		selected_register = (unsigned long *)(mmio + SHA1_22);
		break;
	case DOUT:
		selected_register = (unsigned long *)(mmio + SHA1_23);
		break;
	case RESET:
		selected_register = (unsigned long *)(mmio + SHA1_0);
		break;

	default:
		// printk(KERN_INFO "Error\n");
		break;
	}
	return SUCCESS;
}
/* Module Declarations */
/*
 * This structure will hold the functions to be called
 * when a process does something to the device we
 * created. Since a pointer to this structure is kept in
 * the devices table, it can't be local to
 * init_module. NULL is for unimplemented functions.
 */
struct file_operations Fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.unlocked_ioctl = device_ioctl,
	.open = device_open,
	.release = device_release, /*close */
};

/* Standard module information, edit as appropriate */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Xilinx Inc.");
MODULE_DESCRIPTION("blink - loadable module template generated by petalinux-create -t modules");

#define DRIVER_NAME "Sha1_module"

/* Simple example of how to receive command line parameters to your module.
   Delete if you don't need them */
/*
unsigned myint = 0xdeadbeef;
char *mystr = "default";

module_param(myint, int, S_IRUGO);
module_param(mystr, charp, S_IRUGO);
*/

struct blink_local
{
	int irq;
	unsigned long mem_start;
	unsigned long mem_end;
	void __iomem *base_addr;
};

static irqreturn_t blink_irq(int irq, void *lp)
{
	printk("blink interrupt\n");
	return IRQ_HANDLED;
}

static int blink_probe(struct platform_device *pdev)
{
	int rc = 0;
	struct resource *r_irq; /* Interrupt resources */
	struct resource *r_mem; /* IO mem resources */
	struct device *dev = &pdev->dev;
	struct blink_local *lp = NULL;

	dev_info(dev, "Device Tree Probing\n");

	/* Get iospace for the device */
	r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r_mem)
	{
		dev_err(dev, "invalid address\n");
		return -ENODEV;
	}

	lp = (struct blink_local *)kmalloc(sizeof(struct blink_local), GFP_KERNEL);
	if (!lp)
	{
		dev_err(dev, "Cound not allocate blink device\n");
		return -ENOMEM;
	}

	dev_set_drvdata(dev, lp);

	lp->mem_start = r_mem->start;
	lp->mem_end = r_mem->end;

	if (!request_mem_region(lp->mem_start,
							lp->mem_end - lp->mem_start + 1,
							DRIVER_NAME))
	{
		dev_err(dev, "Couldn't lock memory region at %p\n",
				(void *)lp->mem_start);
		rc = -EBUSY;
		goto error1;
	}

	lp->base_addr = ioremap(lp->mem_start, lp->mem_end - lp->mem_start + 1);
	if (!lp->base_addr)
	{
		dev_err(dev, "blink: Could not allocate iomem\n");
		rc = -EIO;
		goto error2;
	}

	/* Get IRQ for the device */
	r_irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!r_irq)
	{
		dev_info(dev, "no IRQ found\n");
		dev_info(dev, "blink at 0x%08x mapped to 0x%08x\n",
				 (unsigned int __force)lp->mem_start,
				 (unsigned int __force)lp->base_addr);
		return 0;
	}
	lp->irq = r_irq->start;

	rc = request_irq(lp->irq, &blink_irq, 0, DRIVER_NAME, lp);
	if (rc)
	{
		dev_err(dev, "testmodule: Could not allocate interrupt %d.\n",
				lp->irq);
		goto error3;
	}

	dev_info(dev, "blink at 0x%08x mapped to 0x%08x, irq=%d\n",
			 (unsigned int __force)lp->mem_start,
			 (unsigned int __force)lp->base_addr,
			 lp->irq);
	return 0;
error3:
	free_irq(lp->irq, lp);
error2:
	release_mem_region(lp->mem_start, lp->mem_end - lp->mem_start + 1);
error1:
	kfree(lp);
	dev_set_drvdata(dev, NULL);

	return rc;
}

static int blink_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct blink_local *lp = dev_get_drvdata(dev);
	free_irq(lp->irq, lp);
	release_mem_region(lp->mem_start, lp->mem_end - lp->mem_start + 1);
	kfree(lp);
	dev_set_drvdata(dev, NULL);
	return 0;
}

#ifdef CONFIG_OF
static struct of_device_id blink_of_match[] = {
	{
		.compatible = "vendor,blink",
	},
	{/* end of list */},
};
MODULE_DEVICE_TABLE(of, blink_of_match);
#else
#define blink_of_match
#endif

static struct platform_driver blink_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = blink_of_match,
	},
	.probe = blink_probe,
	.remove = blink_remove,
};

static int __init blink_init(void)
{
	int rc = 0;
	printk("<1>Hello module world.\n");
	//printk("<1>Module parameters were (0x%08x) and \"%s\"\n", myint, mystr);

	/*
	 * Register the character device (atleast try)
	 */
	major_num = register_chrdev(0, DEVICE_NAME, &Fops);

	/*
	 * Negative values signify an error
	 */
	if (major_num < 0)
	{
		printk(KERN_ALERT "%s failed with \n", "Sorry, registering the character device ");
	}

	mmio = ioremap(SHA1_BASEADDR, 0x100);

	printk("%s: Registers mapped to mmio = 0x%x  \n", __FUNCTION__, mmio);

	printk(KERN_INFO "%s the major device number is %d.\n", "Registeration is a success", major_num);
	printk(KERN_INFO "If you want to talk to the device driver,\n");
	printk(KERN_INFO "create a device file by following command. \n \n");
	printk(KERN_INFO "mknod %s c %d 0\n\n", DEVICE_NAME, major_num);
	printk(KERN_INFO "The device file name is important, because\n");
	printk(KERN_INFO "the ioctl program assumes that's the file you'll use\n");

	rc = platform_driver_register(&blink_driver);

	return rc;
}

static void __exit blink_exit(void)
{
	unregister_chrdev(major_num, DEVICE_NAME);
	platform_driver_unregister(&blink_driver);
	printk(KERN_ALERT "Goodbye module world.\n");
}

module_init(blink_init);
module_exit(blink_exit);
