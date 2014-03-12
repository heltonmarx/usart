#include <stdio.h>
#include <stdlib.h>

#include "hw_serial.h"

int main(void)
{
    int fd;

    fd = open_serial("/dev/ttyS0", 19200);
    if (fd < 0) {
        fprintf(stderr,"error open_serial\n");
        return -1;
    }


    close_serial(fd);
    return 0;

}
