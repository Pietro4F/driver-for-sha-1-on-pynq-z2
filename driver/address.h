#ifndef HEADER_FILE
#define HEADER_FILE

#include <linux/ioctl.h>

#define MAGIC_NUM 100

#define DIN _IOR(MAGIC_NUM, 0, char *)
#define CV _IOR(MAGIC_NUM, 1, char *)
#define PREV_CV _IOR(MAGIC_NUM, 2, char *)
#define START _IOR(MAGIC_NUM, 3, char *)
#define BUSY _IOR(MAGIC_NUM, 4, char *)
#define VALID _IOR(MAGIC_NUM, 5, char *)
#define DOUT _IOR(MAGIC_NUM, 6, char *)
#define RESET _IOR(MAGIC_NUM, 7, char *)

#define DEBUG

#define DEVICE_FILE_NAME "/dev/Sha1_module"

#endif
