/*
# Copyright 2021 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
*/

/*
* ioctl.c - the process to use ioctl's to control the kernel module
*
* Until now we could have used cat for input and output. But now
* we need to do ioctl's, which require writing our own process.
*/
/*
* device specifics, such as ioctl numbers and the
* major device file.
*/
#include "address.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> /* open */
#include <unistd.h> /* exit */
#include <sys/ioctl.h> /* ioctl */
/*
* Functions for the ioctl calls
*/

int main()
{
	int tmp = 1;
	long r;
	int fd;
	int buf, start_reg;

	int base[16];

	printf("################################ \n\r");
	printf("      SHA1 Application  \n\r");
	printf("################################ \n\r");

	base[15] = 0x61626380;
	base[14] = 0x00000000;
	base[13] = 0x00000000;
	base[12] = 0x00000000;
	base[11] = 0x00000000;
	base[10] = 0x00000000;
	base[9] = 0x00000000;
	base[8] = 0x00000000;
	base[7] = 0x00000000;
	base[6] = 0x00000000;
	base[5] = 0x00000000;
	base[4] = 0x00000000;
	base[3] = 0x00000000;
	base[2] = 0x00000000;
	base[1] = 0x00000000;
	base[0] = 0x00000018;


	fd = open(DEVICE_FILE_NAME, O_RDWR | O_SYNC);
	if (fd < 0)
	{
		printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
		exit(-1);
	}

	printf("First ioctl\n");
	r =  ioctl(fd, START, NULL);
	read(fd, &start_reg, sizeof(int));
	printf("Start register %x\n", start_reg);

	printf("Second ioctl\n");
	r =  ioctl(fd, DIN, NULL);

	if (r < 0) {
		perror ("Unable to open device");
	}

	printf("Writing input data\n");
	write(fd, base, sizeof(int) * 16);
	read(fd, base, sizeof(int)*16);

	for(int i = 0; i < 16; i++){
		printf("Base %d: %x\n", i, base[i]);
	}

	buf = 1;
	printf("Writing start\n");
	r =  ioctl(fd, START, NULL);
	write(fd, &buf, sizeof(int));
	read(fd, &buf, sizeof(int));
	printf("Start register: %x\n", buf);

	buf = 0;
	printf("Resetting start\n");
	write (fd, &buf, sizeof(int));
	read(fd, &buf, sizeof(int));
	printf("Start register: %x\n", buf);

	buf = 0;
	r =  ioctl(fd, VALID, NULL);

	printf("Reading done\n");
	r = read (fd, &buf, sizeof(int));
	read(fd, &buf, sizeof(int));
	printf("Valid register: %x\n", buf);

	printf("Waiting done\n");
	while( (buf & 0x1) == 0){
		r = read (fd, &buf, sizeof(int));
		printf("Read r: %d", buf);
		usleep(100);
	};

	printf("Done asserted\n");
	r =  ioctl(fd, DOUT, NULL);

	printf("Reading output\n");
	r = read (fd, &base[1], sizeof(int)*5);

	printf("Output value: ");
	printf("%08x ", base[5]);
	printf("%08x ", base[4]);
	printf("%08x ", base[3]);
	printf("%08x ", base[2]);
	printf("%08x \n", base[1]);
	close(fd);

    return 0;
}

