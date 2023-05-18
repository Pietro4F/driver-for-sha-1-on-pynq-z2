/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */


#include "platform.h"
#include "xil_io.h"
#include "sha1_IP.h"
#include "xparameters.h"


int main()
{
    init_platform();

    // TEST 1
    xil_printf("Beginning test 1\n");
    //Writing reset register
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG0_OFFSET, 0x00000004);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG0_OFFSET, 0x00000000);

    // Writing the cv
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG21_OFFSET, 0x67452301);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG20_OFFSET, 0xEFCDAB89);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG19_OFFSET, 0x98BADCFE);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG18_OFFSET, 0x10325476);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG17_OFFSET, 0xC3D2E1F0);

    //Writing datain
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG16_OFFSET, 0x61626380);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG15_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG14_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG13_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG12_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG11_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG10_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG9_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG8_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG7_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG6_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG5_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG4_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG3_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG1_OFFSET, 0x00000018);

    //Writing start register
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG0_OFFSET, 0x00000001);
    SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG0_OFFSET, 0x00000000);

    //Waiting for result
    while((SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG22_OFFSET) & 1) == 0);

    //Printing result
    xil_printf("Result is: ");
    xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG27_OFFSET));
    xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG26_OFFSET));
    xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG25_OFFSET));
    xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG24_OFFSET));
    xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG23_OFFSET));
    xil_printf("\n\n");


    // TEST 2
    xil_printf("Beginning test 2\n");
	//Writing reset register
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG0_OFFSET, 0x00000004);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG0_OFFSET, 0x00000000);

	// Writing the cv
	/*
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG21_OFFSET, 0x67452301);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG20_OFFSET, 0xEFCDAB89);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG19_OFFSET, 0x98BADCFE);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG18_OFFSET, 0x10325476);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG17_OFFSET, 0xC3D2E1F0);
	*/

	//Writing datain
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG16_OFFSET, 0x61626364);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG15_OFFSET, 0x62636465);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG14_OFFSET, 0x63646566);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG13_OFFSET, 0x64656667);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG12_OFFSET, 0x65666768);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG11_OFFSET, 0x66676869);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG10_OFFSET, 0x6768696a);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG9_OFFSET, 0x68696a6b);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG8_OFFSET, 0x696a6b6c);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG7_OFFSET, 0x6a6b6c6d);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG6_OFFSET, 0x6b6c6d6e);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG5_OFFSET, 0x6c6d6e6f);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG4_OFFSET, 0x6d6e6f70);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG3_OFFSET, 0x6e6f7071);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG2_OFFSET, 0x80000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG1_OFFSET, 0x00000000);

	//Writing start register
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG0_OFFSET, 0x00000001);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG0_OFFSET, 0x00000000);

	//Waiting for result
	while((SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG22_OFFSET) & 1) == 0);

	//Printing intermediate result
	xil_printf("Obtained intermediate result\n");
	xil_printf("Intermediate result is: ");
	xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG27_OFFSET));
	xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG26_OFFSET));
	xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG25_OFFSET));
	xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG24_OFFSET));
	xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG23_OFFSET));
	xil_printf("\n");

	//Writing datain
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG16_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG15_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG14_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG13_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG12_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG11_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG10_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG9_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG8_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG7_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG6_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG5_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG4_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG3_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG1_OFFSET, 0x000001c0);

	//Writing use_prev_cv register
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG0_OFFSET, 0x00000002);

	//Writing start register
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG0_OFFSET, 0x00000003);
	SHA1_IP_mWriteReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG0_OFFSET, 0x00000002);

	//Waiting for result
	while((SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG22_OFFSET) & 1) == 0);

	//Printing result
	xil_printf("Result is: ");
	xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG27_OFFSET));
	xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG26_OFFSET));
	xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG25_OFFSET));
	xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG24_OFFSET));
	xil_printf("%08x", SHA1_IP_mReadReg(XPAR_SHA1_IP_0_S00_AXI_BASEADDR, SHA1_IP_S00_AXI_SLV_REG23_OFFSET));

    cleanup_platform();
    return 0;
}