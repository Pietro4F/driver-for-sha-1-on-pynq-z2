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

// Defining name of the module
#define DEVICE_NAME "/dev/Sha1_module"

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
#define DEBUG

#endif
