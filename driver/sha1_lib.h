/* 
    Header file to handle the sha1-driver, define the functions prototype and the external variables
*/

#ifndef SHA1_LIB_HEADER_FILE
#define SHA1_LIB_HEADER_FILE


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
// #include <unistd.h>
// #include <sys/ioctl.h>
#include <stdint.h>
#include <errno.h>

// Macro definition
#define DEVICE_FILE_NAME "/dev/Sha1_module"

// Defining magic number to generate ioctl identifiers
#define MAGIC_NUM 100

// // Defining macros to generate identifiers for the ioctl
// #define DIN _IOR(MAGIC_NUM, 0, char *)
// #define CV _IOR(MAGIC_NUM, 1, char *)
// #define PREV_CV _IOR(MAGIC_NUM, 2, char *)
// #define START _IOR(MAGIC_NUM, 3, char *)
// #define BUSY _IOR(MAGIC_NUM, 4, char *)
// #define VALID _IOR(MAGIC_NUM, 5, char *)
// #define DOUT _IOR(MAGIC_NUM, 6, char *)
// #define RESET _IOR(MAGIC_NUM, 7, char *)

#define N_SLAVE_REGISTERS 16
#define SLEEP_TIME 100
#define SUCCESS 0

#define ERROR_FILE -1
#define ERROR_SHA1 -2

#define N_BLOCK 16
#define BLOCK_LENGTH 64
#define DIGESTED_MESS_LENGTH 20

#define START_HASHING_DEF_CV 1
#define START_HASHING_PREV_CV 3

#define L 100 + 1

// Type definitions
typedef struct {
    int code;
    char args[L];
} errorHandler_t;


/*************************   Functions prototype ********************************************/
/** @brief Configure the sha1 device driver
 */
const int sha_1_open();  

/** @brief 
 *  @param  device_addr A pointer to a device file object
 *  @param  input_message A pointer to a file handler that contains the message to encrypt 
 *  @param  digested_message A pointer to the digested message
 */
void sha_1(const int device_addr, FILE* input_message, int* digested_message);  

/** @brief Function to provide commands to the device 
 *  @param  A pointer to a file object (defined in linux/fs.h)
 *  @param  A command
 *  @param  The arguments for the command 
 */
static int send_to_device(const int device_addr, const uint32_t* block, uint32_t command);

/** @brief Function to handle the possible errors
 */
void error();
/********************************************************************************************/

/* External variables definitions */
extern errorHandler_t errorHandler;

#endif