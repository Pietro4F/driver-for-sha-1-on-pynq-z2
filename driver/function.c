#include address.h

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


inn main() {
    int fd = open ("/dev/Sha1_module",O_RDWR	);
	if (fd < 0) {
		perror ("Unable to open device");
	}

    long r =  ioctl(fd, DIN, NULL);

    if (r < 0) {
		perror ("Unable to open device");
	}

    int base[17];

    base[16] = 0x61626380;
    base[15] = 0x00000000;
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
    base[1] = 0x00000018;

    write (fd, &base[1], sizeof(int) * 16);	

    int buf;
    buf = 1;

    long r =  ioctl(fd, START, NULL);
	write (fd, &buf, sizeof(int));	
    buf = 0;
    write (fd, &buf, sizeof(int));	

    long r =  ioctl(fd, VALID, NULL);

    r = read (fd, &buf, sizeof(int));

    while( (*buf & 0x1) == 0){
        r = read (fd, &buf, sizeof(int));
    };

    long r =  ioctl(fd, DOUT, NULL);

    r = read (fd, &base[1], sizeof(int)*5);

    close(fd);

    return 0;
}

/*
int main() {

	int fd = open ("/dev/LM_module",O_RDWR	);
	if (fd < 0) {
		perror ("Unable to open device");
	}

	unsigned int arg;
	long r =  ioctl(fd, 0x01 , &arg);

	if (r < 0) {
		perror ("Unable to open device");
	}

	int buf;
	r = read (fd, &buf, sizeof(int));

	write (fd, &buf, sizeof(int));	

	close(fd);

}*/
