#include "sha1_lib.h"

errorHandler_t errorHandler;

const int sha_1_open(){
    	// const int device = open(DEVICE_FILE_NAME, O_RDWR | O_SYNC);
        // if(device < 0){
        //         //Error during opening
        //         //close();

        //         errorHandler.code = ERROR_SHA1;
        //         sprintf(errorHandler.args, "Error in sha1_lib during open execution!\n");
        //         }

        // return device;
}  

static int send_to_device(const int device_addr, const uint32_t* block, uint32_t command) {
	
	uint8_t i;
	uint32_t inverted_block[N_SLAVE_REGISTERS];
    uint32_t status_register;
	
	for(i=0;i<N_SLAVE_REGISTERS;i++) {
		inverted_block[i] =	block[(N_SLAVE_REGISTERS-1)-i];
	}

	// // Selecting the input command register
	// if(ioctl(device_addr, DIN, NULL) < 0){
	//  	errorHandler.code = ERROR_SHA1;
    //  	sprintf(errorHandler.args, "Error in sha1_lib during ioctl execution! Not a valid address.\n");
	// }

	// // Writing the input data to the device
	// write(device_addr, inverted_block, N_SLAVE_REGISTERS*sizeof(uint32_t));		   

	// // Selecting the input command register
	// if(ioctl(device_addr, START, NULL) < 0){
	//  	errorHandler.code = ERROR_SHA1;
    //  	sprintf(errorHandler.args, "Error in sha1_lib during ioctl execution! Not a valid address.\n");
	// }


	// // Starting the hashing
	// write(device_addr, &command, sizeof(uint32_t));				

	// // Selecting the status register
	// if(ioctl(device_addr, VALID, NULL) < 0){
	//  	errorHandler.code = ERROR_SHA1;
	//  	sprintf(errorHandler.args, "Error in sha1_lib during ioctl execution! Not a valid address.\n");
	//  }	

    // // Reading status register
    // read(device_addr, &status_register, sizeof(uint32_t));			

    // // Checking if the device has finished
    // while( (status_register & 0x1) == 0){			
    //     read(device_addr, &status_register, sizeof(uint32_t));
    //     usleep(SLEEP_TIME);
    //     }

    // // Selecting the input command register
    // if(ioctl(device_addr, START, NULL) < 0){
    // errorHandler.code = ERROR_SHA1;
    // sprintf(errorHandler.args, "Error in sha1_lib during ioctl execution! Not a valid address.\n");
    // }

	// command = 0;
	// write (device_addr, &command, sizeof(uint32_t));

	return SUCCESS;
}



void sha_1(const int device_addr, FILE* input_message, int* digested_message) {

	
 	uint8_t i;

	uint16_t read_return;

	uint32_t block[N_BLOCK];
	uint32_t command, status_register;

	uint64_t file_length;

	/* Step1: File length evaluation*/
	// Reaching end of the message
	fseek(input_message, 0L, SEEK_END);	

	// Counting the bytes in the message
	file_length = ftell(input_message);			
	file_length = file_length << 3;

	// Returning to the beginning of the message
	fseek(input_message, 0L, SEEK_SET);					


	/* Step2: Preprocessing and hashing */	
	// Command to start the hashing using the original CV
	command = START_HASHING_DEF_CV;										

	// Trying to read 512 bits (first block) from the message
	read_return = fread(block, sizeof(char), BLOCK_LENGTH, input_message);		
	
	// Checking if the block have been successfully readed
	while(read_return == BLOCK_LENGTH) {							

		send_to_device(device_addr, block, command);

		// Command to start hashing using the CV generated by the previous iteration
		command = START_HASHING_PREV_CV;

		// Trying to read the next block of the message
		read_return = fread(block, sizeof(char), BLOCK_LENGTH, input_message);	
	}				


	// Checking if the last readed block requires an additional block for the padding
	if ((read_return << 3) < 447) {							
	
		// CASE A: No additional block required

		// Add termination byte '1000_0000'
		*((uint8_t*)(block)+read_return) = 0x80;
		
		// Zero padding
		for(i = (read_return+1); i < 56; i++) {
			*((uint8_t*)(block)+i) = 0x00;
		}


	} else {			

		// CASE B: Additional block required

		// Add termination byte '1000_0000'
		*((uint8_t*)(block)+read_return) = 0x80;
		
		// Zero padding till the end
		for(i = (read_return+1); i < 64; i++) {
			*((uint8_t*)(block)+i) = 0x00;
		}

		// Endianess correction (VERIFICARE PERCHE C'ERA 14 ORA E' 15)
		for (i=0; i<(N_BLOCK - 1); i++) {
			block[i] = __builtin_bswap32(block[i]);
		}

		// Send the message to the sha1 block
		send_to_device(device_addr, block, START_HASHING_PREV_CV);

		// Zero padding in the additional block
		for(i = 0; i < 56; i++) {
			*((uint8_t*)(block)+i) = 0x00;
		}

	}

	// Add the message length at the end
	block[14] = file_length >> 32;
	block[15] = file_length;

	// Endianess correction 
	for (i=0; i<(N_BLOCK - 1); i++) {
		block[i] = __builtin_bswap32(block[i]);
	}

	// Send the message to the sha1 block
	if( file_length < 447){
		send_to_device(device_addr, block, START_HASHING_DEF_CV);
	} else {
		send_to_device(device_addr, block, START_HASHING_PREV_CV);
	}

	// // Selecting the input command register
	// if(ioctl(device_addr, DOUT, NULL) < 0){
	//  	errorHandler.code = ERROR_SHA1;
	//  	sprintf(errorHandler.args, "Error in sha1_lib during ioctl execution! Not a valid address.\n");
	//  }

	// // Read the digested message
	// read(device_addr, digested_message, DIGESTED_MESS_LENGTH);

}

void error(){
    switch (errorHandler.code){
        case ERROR_FILE:
            fprintf(stderr, "Error opening file %s \n", errorHandler.args);
            break;
        case ERROR_SHA1:
            fprintf(stderr, "Error in sha1! %s \n", errorHandler.args);
            break;
        default:
            break;
    }
}

