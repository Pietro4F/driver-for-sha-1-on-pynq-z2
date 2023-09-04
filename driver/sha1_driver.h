/*

This code is licensed under a Creative Commons Attribution-NonCommercial 4.0 International License.
https://creativecommons.org/licenses/by-nc/4.0/

Developed by:
- Pietro Fagnani	Politecnico di Torino, Italy
- Luca D'Elia		Politecnico di Torino, Italy
- Marco Massetti	Politecnico di Torino, Italy
- Jacopo Serra		Politecnico di Torino, Italy
- Massimo Emiliano	Politecnico di Torino, Italy
- Gioele Sineo		Politecnico di Torino, Italy

There is no warranty

*/

#ifndef SHA1_HEADER_FILE
#define SHA1_HEADER_FILE


#include <linux/init.h>		// Macros used to mark up functions
#include <linux/module.h>	// Core header for loading LKMs into the kernel
#include <linux/device.h> 	// Header to support the kernel Driver Model
#include <linux/kernel.h>	// Contains types, macros, functions for the kernel
#include <linux/fs.h>		// Header for the Linux file system support
#include <asm/uaccess.h>	// Required for the copy to user function
#include <asm/io.h>			// Required for the ioremap function


// Defining magic number to generate ioctl identifiers
#define MAGIC_NUM 100

// Defining macros to generate identifiers for the ioctl
#define DIN _IOR(MAGIC_NUM, 0, char *)
#define CV _IOR(MAGIC_NUM, 1, char *)
#define PREV_CV _IOR(MAGIC_NUM, 2, char *)
#define START _IOR(MAGIC_NUM, 3, char *)
#define BUSY _IOR(MAGIC_NUM, 4, char *)
#define VALID _IOR(MAGIC_NUM, 5, char *)
#define DOUT _IOR(MAGIC_NUM, 6, char *)
#define RESET _IOR(MAGIC_NUM, 7, char *)

// Defining return codes
#define SUCCESS 0
#define DEVICE_BUSY 1
#define IOCTL_FAIL 2

// Defining name of the module and class name
#define DEVICE_NAME "sha1_module"
#define CLASS_NAME  "sha1"

// Defining base address for the registers of the device
#define SHA1_BASEADDR 0x43C00000

// Defining offsets for the registers of the device
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

// If "DEBUG" is defined additional information are printed 
#define DEBUG 0


/** @brief This is called whenever a process attempts to open the device file. 
 *  The hardware is initialized
 *  @param inode A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @return Returns 0 if successful
 */
static int sha1_open(struct inode *inode, struct file *file);


/** @brief This function is called whenever a process which has already opened the
 *  device file attempts to read from it.
 *  Starting from the address selected using the ioctl the device registers are readed for the length
 *  of the buffer specified as function argument.
 *  @param file A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param length The length of the buffer
 *  @param offset The offset if required
 *  @return Returns the number of bytes that have been correctly readed
 */
static ssize_t sha1_read(struct file *file, char __user *buffer, size_t length, loff_t *offset);


/** @brief This function is called when somebody tries to
 *  write into our device file.
 *  Starting from the address selected using the ioctl the device registers are writen for the length
 *  of the buffer specified as function argument.
 *  @param file A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param length The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 *  @return Returns the number of bytes that have been correctly written
 */
static ssize_t sha1_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset);


/** @brief Function to provide commands to the device 
 *  Using this function it is possible to select a register to target for the read/write operations
 *  @param file A pointer to a file object (defined in linux/fs.h)
 *  @param ioctl_num A command
 *  @param ioctl_param The arguments for the command 
 *  @return Returns 0 if successful
 */
long sha1_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);


/** @brief This is called whenever a process attempts to close the device file.
 *  The hardware is resetted
 *  @param inode A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @return Returns 0 if successful
 */
static int sha1_release(struct inode *inode, struct file *file);


/** @brief The LKM initialization function
 *  @return Returns 0 if successful
 */
static int __init sha1_init(void);


/** @brief The LKM cleanup function
 */
static void __exit sha1_exit(void);


#endif
