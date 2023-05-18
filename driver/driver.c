#include "address.h"


#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>          // Required for the copy to user function
#define  DEVICE_NAME "Sha1_module"    ///< The device will appear at /dev/ebbchar using this value
#define  CLASS_NAME  "Sha1"        ///< The device class -- this is a character device driver
 
MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("Stefano Di Carlo");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux char driver for an emulated LED device");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users
 
static int    majorNumber;                  ///< Stores the device number -- determined automatically
static struct class*  Sha1charClass  = NULL; ///< The device-driver class struct pointer
static struct device* Sha1charDevice = NULL; ///< The device-driver device struct pointer
 
// The prototype functions for the character driver -- must come before the struct definition

volatile unsigned *selected_register;

static int     Sha1_open_close(struct inode *, struct file *);
static ssize_t Sha1_read(struct file *, char *, size_t, loff_t *);
static ssize_t Sha1_write(struct file *, const char *, size_t, loff_t *);
static long int Sha1_ioctl(struct file *, unsigned int , unsigned long );



/** @brief Devices are represented as file structure in the kernel. The file_operations structure from
 *  /linux/fs.h lists the callback functions that you wish to associated with your file operations
 *  using a C99 syntax structure. char devices usually implement open, read, write and release calls
 */
static struct file_operations fops =
{
   .owner = THIS_MODULE,	
   .open = Sha1_open_close,
   .read = Sha1_read,
   .write = Sha1_write,
   .release = Sha1_open_close,
   .unlocked_ioctl = Sha1_ioctl,
   .compat_ioctl = Sha1_ioctl
};


/** @brief Function to provide commands to the device 
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param cmd A command
 *  @param arg The arguments for the command 
 */

static long int Sha1_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	
    switch(cmd) {
        case DIN:
            selected_register = (int *)(SHA1_BASEADDR + SHA1_1);
            break;
        case CV:
            selected_register = (int *)(SHA1_BASEADDR + SHA1_17);
            break;
        case PREV_CV:
            selected_register = (int *)(SHA1_BASEADDR + SHA1_0);
            break;
        case START:
            selected_register = (int *)(SHA1_BASEADDR + SHA1_0);
            break;
        case BUSY:
            selected_register = (int *)(SHA1_BASEADDR + SHA1_22);
            break;
        case VALID:
            selected_register = (int *)(SHA1_BASEADDR + SHA1_22);
            break;
        case DOUT:
            selected_register = (int *)(SHA1_BASEADDR + SHA1_23);
            break;
        case RESET:
            selected_register = (int *)(SHA1_BASEADDR + SHA1_0);
            break;

        default :
            //printk(KERN_INFO "Error\n");
            break;
	    
	}
   	//printk(KERN_INFO "Sha1: Executing IOCTL\n");
	
	return -1;
}


/** @brief The device release function that is called whenever the device is initialized/released by
 *  the userspace program
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */

static int  Sha1_open_close(struct inode* inodep, struct file * filep) {
	
    selected_register = (int *)(SHA1_BASEADDR + SHA1_0);
    *selected_register = 0x4;
    *selected_register = 0x0;

    selected_register = (int *)(SHA1_BASEADDR + SHA1_17);
    *selected_register = 0x67452301;
    
    selected_register = (int *)(SHA1_BASEADDR + SHA1_18);
    *selected_register = 0xEFCDAB89;
    
    selected_register = (int *)(SHA1_BASEADDR + SHA1_19);
    *selected_register = 0x98BADCFE;
    
    selected_register = (int *)(SHA1_BASEADDR + SHA1_20);
    *selected_register = 0x10325476;
    
    selected_register = (int *)(SHA1_BASEADDR + SHA1_21);
    *selected_register = 0xC3D2E1F0;
	
   	//printk(KERN_INFO "Sha1: Executing OPENRELEASE\n");
	return 0;

}


/** @brief This function is called whenever device is being read from user space i.e. data is
 *  being sent from the device to the user. In this case is uses the copy_to_user() function to
 *  send the buffer string to the user and captures any errors.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param len The length of the b
 *  @param offset The offset if required
 */
static ssize_t Sha1_read(struct file * filep, char * buffer, size_t len, loff_t * offset) {
	

    return copy_to_user(buffer, selected_register, 4);
    
    
    /*int data;
	READ_DATA_FROM_THE_HW (&data);
	copy_to_user(buffer, &data, 4);*/

   	//printk(KERN_INFO "Sha1: Executing READ\n");
	//return 4;
}


/** @brief This function is called whenever the device is being written to from user space i.e.
 *  data is sent to the device from the user. 
 *  @param file A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param len The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 */
static ssize_t Sha1_write(struct file * filep, const char * buffer, size_t len, loff_t * offset) {

    int i;
    for (i=0; i< (len%4); i++){
        *(selected_register + (i*4))= buffer[i];
    }

	//WRITE_DATA_TO_THE_HW( buffer);
   	//printk(KERN_INFO "Sha1: Executing WRITE\n");
	return 1;	
}




/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point.
 *  @return returns 0 if successful
 */


static int __init Sha1_module_init(void){
   printk(KERN_INFO "Sha1: Initializing the Sha1ODULE LKM\n");
 
   // Try to dynamically allocate a major number for the device -- more difficult but worth it
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "Sha1 failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "Sha1: registered correctly with major number %d\n", majorNumber);
 
   // Register the device class
   Sha1charClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(Sha1charClass)){                // Check for error and clean up if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(Sha1charClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "Sha1: device class registered correctly\n");
 
   // Register the device driver
   Sha1charDevice = device_create(Sha1charClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(Sha1charDevice)){               // Clean up if there is an error
      class_destroy(Sha1charClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(Sha1charDevice);
   }
   printk(KERN_INFO "Sha1: device class created correctly\n"); // Made it! device was initialized
   return 0;
}
 
/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static. The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit Sha1_module_exit(void){
   device_destroy(Sha1charClass, MKDEV(majorNumber, 0));     // remove the device
   class_unregister(Sha1charClass);                          // unregister the device class
   class_destroy(Sha1charClass);                             // remove the device class
   unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
   printk(KERN_INFO "Sha1: Goodbye from the LKM!\n");
}
 
 
 
 
 
/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(Sha1_module_init);
module_exit(Sha1_module_exit);



struct blink_local {
	int irq;
	unsigned long mem_start;
	unsigned long mem_end;
	void __iomem *base_addr;
};


static struct platform_driver blink_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table	= blink_of_match,
	},
	.probe		= blink_probe,
	.remove		= blink_remove,
};


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
	if (!r_mem) {
		dev_err(dev, "invalid address\n");
		return -ENODEV;
	}
	
	lp = (struct blink_local *) kmalloc(sizeof(struct blink_local), GFP_KERNEL);
	if (!lp) {
		dev_err(dev, "Cound not allocate blink device\n");
		return -ENOMEM;
	}
	
	dev_set_drvdata(dev, lp);
	
	lp->mem_start = r_mem->start;
	lp->mem_end = r_mem->end;

	if (!request_mem_region(lp->mem_start,
				lp->mem_end - lp->mem_start + 1,
				DRIVER_NAME)) {
		dev_err(dev, "Couldn't lock memory region at %p\n",
			(void *)lp->mem_start);
		rc = -EBUSY;
		goto error1;
	}

	lp->base_addr = ioremap(lp->mem_start, lp->mem_end - lp->mem_start + 1);
	if (!lp->base_addr) {
		dev_err(dev, "blink: Could not allocate iomem\n");
		rc = -EIO;
		goto error2;
	}

	/* Get IRQ for the device */
	r_irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!r_irq) {
		dev_info(dev, "no IRQ found\n");
		dev_info(dev, "blink at 0x%08x mapped to 0x%08x\n",
			(unsigned int __force)lp->mem_start,
			(unsigned int __force)lp->base_addr);
		return 0;
	}
	lp->irq = r_irq->start;
	
	rc = request_irq(lp->irq, &blink_irq, 0, DRIVER_NAME, lp);
	if (rc) {
		dev_err(dev, "testmodule: Could not allocate interrupt %d.\n",
			lp->irq);
		goto error3;
	}

	dev_info(dev,"blink at 0x%08x mapped to 0x%08x, irq=%d\n",
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





