/* 
    Header file to handle the sha1-driver, define the functions prototype and the external variables
*/

#ifndef SHA1_LIB_HEADER_FILE
#define SHA1_LIB_HEADER_FILE

//#include "driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
//#include <sys/ioctl.h>
#include <stdint.h>
#include <errno.h>

// Macro definition
#define N_SLAVE_REGISTERS 16
#define SLEEP_TIME 100
#define SUCCESS 0

#define ERROR_FILE -1
#define ERROR_SHA1 -2

#define N_BLOCK 16
#define BLOCK_LENGTH 512
#define DIGESTED_MESS_LENGTH 20

#define START_HASHING_DEF_CV 1
#define START_HASHING_PREV_CV 1

#define L 100 + 1

// Type definitions
typedef struct {
    int code;
    char args[L];
} errorHandler_t;


/*************************   Functions prototype ********************************************/
/** @brief Apply the SHA1 (Secure Hash Algorithm 1) to the 'input_message'
 *  @param  device_addr A pointer to a device file object
 *  @param  input_message A pointer to a file handler that contains the message to encrypt 
 *  @param  digested_message A pointer to the digested message
 */
void sha_1(const int device_addr, FILE* input_message, int* digested_message);  

/** @brief Send a block (512 bits with zero padding and coded message lenght) to the SHA1 device and
 * relative command in order to starts the encoding 
 *  @param device_addr A pointer to the device object 
 *  @param command A command for the control register
 *  @param block Part of the message to encrypt 
 */
static int send_to_device(const int device_addr, const uint32_t* block, uint32_t command);

/** @brief Function to handle the possible errors
 */
void error();
/********************************************************************************************/

/* External variables definitions */
extern errorHandler_t errorHandler;

#endif