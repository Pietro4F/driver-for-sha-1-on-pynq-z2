#include "sha1_driver.h"


//Module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("OS_2023_TEAM_4");
MODULE_DESCRIPTION("SHA1 - driver to control sha1 hardware module");
MODULE_VERSION("1.0");


static unsigned long *mmio;						// Address of the first register of the device
static unsigned long *selected_register;		// Address of the register selected using ioctl for read/write operations
static int Device_Open = 0;						// Flag to signal if device is already open. Used to prevent concurent access into the same device


static int major_num;							// Major number of the device
static struct class*  sha1_charClass  = NULL; 	// The device-driver class struct pointer
static struct device* sha1_charDevice = NULL; 	// The device-driver device struct pointer


/*
 * Structure that holds the functions to be called
 * when a process interacts with the device.
 */
struct file_operations Fops = {
	.owner = THIS_MODULE,
	.open = sha1_open,
	.read = sha1_read,
	.write = sha1_write,
	.unlocked_ioctl = sha1_ioctl,
	.release = sha1_release
};


static int sha1_open(struct inode *inode, struct file *file) {
	
	if(DEBUG == 1) {
		printk(KERN_INFO "Sha1: Executing OPEN\n");
	}

	// Check if one other process is already accessing the device
	if (Device_Open == 1) {

		// If device accessed by other process an error code is returned
		return -DEVICE_BUSY;
	} else {

		// If no other process is using the device the flag is set
		Device_Open = 1;
	}

	// Sending a reset signal pulse to the machine
	selected_register = (unsigned long *)(mmio + SHA1_0);
	*selected_register = 0x4;
	*selected_register = 0x0;

	// Setting the initial hash value
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

	return SUCCESS;
}


static ssize_t sha1_read(struct file *file, char __user *buffer, size_t length, loff_t *offset) {
	
	// Bytes of data readed from the device
	ssize_t bytes_readed = 0;

	// Address from where data will be readed
	static unsigned long *func_register;
	
	// Setting initial address to the value selected by ioctl 
	func_register = selected_register;
	
	// Reading until the input buffer is full
    while(length != 0) {
		
		if(DEBUG == 1) {
			printk(KERN_INFO "Sha1: Executing READ: selected register %lu, value %x\n", func_register, *func_register);
		}
        
		// Reading one word from the device
        if(copy_to_user(buffer, func_register, 4) == 0){

			// If successfully copied the word to user space
            
			// Updating the pointer of the input buffer
			buffer = buffer + 4;

			// Updating the word of memory to read next
			func_register = func_register + 1;

			// Updating the number of byter that have to be readed
            length = length - 4;

			// Updating the number of bytes that have been correctly readed
            bytes_readed = bytes_readed + 4;

        } else{

			// If an error occurred the reading is stopped
            break;
        }
    }

	// Returning the number of bytes that have been correctly readed
    return bytes_readed;
}


static ssize_t sha1_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset) {
	
	// Bytes of data written from the device
	ssize_t bytes_written = 0;

	// Address from where data will be readed
	static unsigned long *func_register;
	
	// Setting initial address to the value selected by ioctl 
	func_register = selected_register;
	
    while(length != 0) {
	
		// Writing one word to the device
        if(copy_from_user(func_register, buffer, 4) == 0){

			if(DEBUG == 1) {
				printk(KERN_INFO "Sha1: Executing WRITE: selected register %lu, value %x\n", func_register, *func_register);
        	}

            // If successfully copied the word to the device

			// Updating the pointer of the input buffer
			buffer = buffer + 4;

			// Updating the word of memory to write next
			func_register = func_register + 1;

			// Updating the number of byter that have to be writed
            length = length - 4;

			// Updating the number of bytes that have been correctly written
            bytes_written = bytes_written + 4;
        } else{

			// If an error occurred the writing is stopped
            break;
        }
    }

	// Returning the number of bytes that have been correctly written
	return bytes_written;
}


long sha1_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param) {

	if(DEBUG == 1) {
		printk(KERN_INFO "Sha1: Executing IOCTL: command %x\n", ioctl_num);
    }

	// Switch according to the ioctl command
	switch (ioctl_num) {
		 
		case DIN:
		// First register for the input data
		selected_register = (unsigned long *)(mmio + SHA1_1);
		break;

		case CV:
		// Fistr register for the initial hash
		selected_register = (unsigned long *)(mmio + SHA1_17);
		break;

		case PREV_CV:
		// Controll register
		selected_register = (unsigned long *)(mmio + SHA1_0);
		break;

		case START:
		// Controll register
		selected_register = (unsigned long *)(mmio + SHA1_0);
		break;

		case BUSY:
		// Status register
		selected_register = (unsigned long *)(mmio + SHA1_22);
		break;

		case VALID:
		// Status register
		selected_register = (unsigned long *)(mmio + SHA1_22);
		break;

		case DOUT:
		// First register for the output data
		selected_register = (unsigned long *)(mmio + SHA1_23);
		break;

		case RESET:
		// Controll register
		selected_register = (unsigned long *)(mmio + SHA1_0);
		break;

		default:
		// Invalid command
		return -IOCTL_FAIL;
		break;
	}

	return SUCCESS;
}


static int sha1_release(struct inode *inode, struct file *file) {

	if(DEBUG == 1) {
		printk(KERN_INFO "Sha1: Executing RELEASE\n");
	}

	// Resetting the flag
	Device_Open = 0;
	
	// Sending a reset signal pulse to the machine
	selected_register = (unsigned long *)(mmio + SHA1_0);
	*selected_register = 0x4;
	*selected_register = 0x0;

	return SUCCESS;
}


static int __init sha1_init(void) {

	if(DEBUG == 1) {
		printk(KERN_INFO "Sha1: Executing INIT\n");
	}

	// Registering the character device dynamically allocating a major number
	major_num = register_chrdev(0, DEVICE_NAME, &Fops);

	// Checking if an error has occurred
	if (major_num < 0) {
		
		printk(KERN_ALERT "Sha1: Failed to register a major number\n");
		return major_num;
	}

	// Registering the device class
	sha1_charClass = class_create(THIS_MODULE, CLASS_NAME);

	// Checking if an error has occurred
	if (IS_ERR(sha1_charClass)) {

		unregister_chrdev(major_num, DEVICE_NAME);
		printk(KERN_ALERT "Sha1: Failed to register device class\n");
		return PTR_ERR(sha1_charClass);
	}

	// Register the device driver
	sha1_charDevice = device_create(sha1_charClass, NULL, MKDEV(major_num, 0), NULL, DEVICE_NAME);

	// Checking if an error has occurred
	if (IS_ERR(sha1_charDevice)) {
		
		class_destroy(sha1_charClass);
		unregister_chrdev(major_num, DEVICE_NAME);
		printk(KERN_ALERT "Sha1: Failed to create the device\n");
		return PTR_ERR(sha1_charDevice);
	}

	// Getting virtual address that corresponds to the first memory (physical) address of the device (second argument is the size of the address space)
	mmio = ioremap(SHA1_BASEADDR, 0x100);

	if(DEBUG == 1) {
		printk("Sha1: Registers mapped to %x\n", mmio);
	}

	printk(KERN_INFO "Sha1: Registeration is a success the major device number is %d.\n", major_num);
	
	return 0;
}


static void __exit sha1_exit(void) {

	if(DEBUG == 1) {
		printk(KERN_ALERT "Sha1: Executing EXIT\n");
	}

	// Removing the device
	device_destroy(sha1_charClass, MKDEV(major_num, 0));

	// Unregistering the device class
	class_unregister(sha1_charClass);

	// Removing the device class
	class_destroy(sha1_charClass);

	// Unregistering the major number
	unregister_chrdev(major_num, DEVICE_NAME);

	printk(KERN_INFO "Sha1: Successfully removed device\n");
}


module_init(sha1_init);
module_exit(sha1_exit);
