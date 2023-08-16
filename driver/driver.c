#include "driver.h"


//Module information
MODULE_LICENSE("CC-BY-NC-4.0");
MODULE_AUTHOR("OS_2023_TEAM_4");
MODULE_DESCRIPTION("SHA1 - driver to control sha1 hardware module");


// Address of the first register of the device
static unsigned long *mmio;

// Address of the register selected using ioctl for read/write operations
static unsigned long *selected_register;

// Major number of the device
static int major_num;

// Flag to signal if device is already open. Used to prevent concurent access into the same device
static int Device_Open = 0;


/** @brief This is called whenever a process attempts to open the device file. 
 *  The hardware is initialized
 *  @param inode A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int sha1_open(struct inode *inode, struct file *file) {
	
	#ifdef DEBUG
		printk(KERN_INFO "Sha1: Executing OPEN\n");
	#endif

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


/** @brief This function is called whenever a process which has already opened the
 *  device file attempts to read from it.
 *  Starting from the address selected using the ioctl the device registers are readed for the length
 *  of the buffer specified as function argument.
 *  @param file A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param length The length of the buffer
 *  @param offset The offset if required
 */
static ssize_t sha1_read(struct file *file, char __user *buffer, size_t length, loff_t *offset) {
	
	// Bytes of data readed from the device
	ssize_t bytes_readed = 0;

	// Address from where data will be readed
	static unsigned long *func_register;
	
	// Setting initial address to the value selected by ioctl 
	func_register = selected_register;
	
	// Reading until the input buffer is full
    while(length != 0) {
		
		#ifdef DEBUG
			printk(KERN_INFO "Sha1: Executing READ: selected register %lu, value %x\n", func_register, *func_register);
		#endif
        
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

	// Returning the number of byter that have been correctly readed
    return bytes_readed;
}


/** @brief This function is called when somebody tries to
 *  write into our device file.
 *  Starting from the address selected using the ioctl the device registers are writen for the length
 *  of the buffer specified as function argument.
 *  @param file A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param length The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 */
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

			#ifdef DEBUG
				printk(KERN_INFO "Sha1: Executing WRITE: selected register %lu, value %x\n", func_register, *func_register);
        	#endif

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

	// Returning the number of byter that have been correctly written
	return bytes_written;
}


/** @brief Function to provide commands to the device 
 *  Using this function it is possible to select a register to target for the red/write operations
 *  @param file A pointer to a file object (defined in linux/fs.h)
 *  @param ioctl_num A command
 *  @param ioctl_param The arguments for the command 
 */
long sha1_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param) {

	#ifdef DEBUG
		printk(KERN_INFO "Sha1: Executing IOCTL: command %x\n", ioctl_num);
    #endif

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


/** @brief This is called whenever a process attempts to close the device file.
 *  The hardware is resetted
 *  @param inode A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int sha1_release(struct inode *inode, struct file *file) {

	#ifdef DEBUG
		printk(KERN_INFO "Sha1: Executing RELEASE\n");
	#endif

	// Resetting the flag
	Device_Open = 0;
	
	// Sending a reset signal pulse to the machine
	selected_register = (unsigned long *)(mmio + SHA1_0);
	*selected_register = 0x4;
	*selected_register = 0x0;

	return SUCCESS;
}


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


static int __init sha1_init(void) {

	#ifdef DEBUG
		printk(KERN_INFO "Sha1: Executing INIT\n");
	#endif

	// Registering the character device dynamically allocating a major number
	major_num = register_chrdev(0, DEVICE_NAME, &Fops);

	// Checking if an error has occurred
	if (major_num < 0) {
		
		printk(KERN_ALERT "Sha1: Registering the character device failed\n");
		return major_num;
	}

	// Getting virtual address that corresponds to the first memory (physical) address of the device (second argument is the size of the address space)
	mmio = ioremap(SHA1_BASEADDR, 0x100);

	#ifdef DEBUG
		printk("Sha1: Registers mapped to %x\n", mmio);
	#endif

	printk(KERN_INFO "Sha1: Registeration is a success the major device number is %d.\n", major_num);
	
	return 0;
}


static void __exit sha1_exit(void) {

	#ifdef DEBUG
		printk(KERN_ALERT "Sha1: Executing EXIT\n");
	#endif

	// Unregistering the major number
	unregister_chrdev(major_num, DEVICE_NAME);
}


module_init(sha1_init);
module_exit(sha1_exit);
