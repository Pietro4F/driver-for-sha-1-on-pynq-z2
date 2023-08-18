#include <stdio.h>
#include <stdlib.h>
#include "sha1_lib.h"

int main() {

	int fd;

	// Open file to encrypt
	fd = open(DEVICE_FILE_NAME, O_RDONLY);

    // // Open the sha1 driver
	// const int device = sha_1_open();

    // Perform the encryption
	sha_1(0, fd, NULL);

    // Execute error function
    error();

	return 0;
}
