#include <stdio.h>
#include <stdlib.h>
#include "./sha1_lib_v1.h"

int main() {

	FILE* fd;

	// Open file 
	fd = fopen("test.txt", "r");

	//const int device = open(DEVICE_FILE_NAME, O_RDWR | O_SYNC);
	sha_1(0, fd, NULL);

	return 0;
}
