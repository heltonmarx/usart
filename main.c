#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <unistd.h>

#include "hw_serial.h"

typedef struct {
    int baudrate;
    char device[64];
    int fd;
} usartData_t;

static struct argp_option options[] = {
    {"baudrate",    'b',    "NUM",      0,  "Baudrate. e.g: 38400",     0},
    {"device",      'd',    "STRING",   0,  "Device.e.g:/dev/ttyS0",    0},
    /* end of table */
    {NULL,          0,      NULL,       0,  NULL,                       0},
};

static int parse_opt (int key, char *arg, struct argp_state *state)
{
    usartData_t *u = (usartData_t*)state->input;
    switch (key) {
    case 'b':   /** baudrate */
        if ((arg != NULL) && strlen(arg)) {
            u->baudrate = atoi(arg);
        }
        break;
    case 'd':   /** device */
        if ((arg != NULL) && strlen(arg)) {
            strncpy(u->device, arg, sizeof(u->device));
        }
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp parser = {
    options,
    parse_opt,
    NULL,
    "usart echo\n",
    NULL,
    NULL,
    NULL
};

int main(int argc, char *argv[])
{
    usartData_t usart;
    unsigned char buf[2048];
    int ret;

    memset(&usart, 0x00, sizeof(usart));
    argp_parse(&parser, argc, argv, 0, 0, &usart);

    if (usart.baudrate != 0)  {
        usart.fd = open_serial(usart.device, usart.baudrate);
        if (usart.fd < 0) {
            return -1;
        }
        while(1) {
            ret = read_serial(usart.fd, buf, sizeof(buf), 1000);
            if (ret > 0) {
                write_serial(usart.fd, buf, ret);
            }
        }
    }
    return 0;
}
