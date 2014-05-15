#include <hw_serial.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

struct _baudrate_info {
    int bps;
    int bdrate;
};

const struct _baudrate_info baudrate_list[] = {
    { 50, B50 },
    { 75, B75 },
    { 110, B110 },
    { 134, B134 },
    { 150, B150 },
    { 200, B200 },
    { 300, B300 },
    { 600, B600 },
    { 1200, B1200 },
    { 1800, B1800 },
    { 2400, B2400 },
    { 4800, B4800 },
    { 9600, B9600 },
    { 19200, B19200 },
    { 38400, B38400 },
    { 57600, B57600 },
    { 115200, B115200 },
    { 230400, B230400 },
};

static size_t get_bps_list_size()
{
    return (sizeof(baudrate_list) / sizeof(struct _baudrate_info));
}

int open_serial(const char *dev, int bps)
{
    struct termios config;
    int bdrate = B19200;
    int fd;
    size_t i;

    if (!dev || !strlen(dev)) {
        fprintf(stderr,"device error: %s\n", dev);
        return -1;
    }
    fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
    /* fclose error */
    if (fd < 0) {
        fprintf(stderr,"open [open] serial error: %s\n", strerror(errno));
        return -1;
    }
    fcntl(fd, F_SETFL, 0);
    memset(&config, 0, sizeof(struct termios));

    for(i = 0; i < get_bps_list_size(); i++) {
        if ( baudrate_list[i].bps == bps ) {
            bdrate =  baudrate_list[i].bdrate;
            break;
        }
    }
    cfsetospeed(&config, bdrate);
    cfsetispeed(&config, bdrate);

    /* no partity (8N1) */
    config.c_cflag &= ~PARENB;
    config.c_cflag &= ~CSTOPB;
    config.c_cflag &= ~CSIZE;
    config.c_cflag |= CS8;

    /* enable raw input */
    config.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /* raw output */
    config.c_oflag &= ~OPOST;

    /* no flow control... */
    config.c_iflag &= ~(IXON | IXOFF | IXANY);
    config.c_cflag |= (CLOCAL|CREAD);
    config.c_cc[VMIN] = 0;
    config.c_cc[VTIME] = 255;

    tcflush(fd, TCIOFLUSH);
    if (tcsetattr(fd, TCSANOW, &config) < 0) {
        fprintf(stderr,"open [tcsetattr] serial error: %s\n", strerror(errno));
        close(fd);
        return -1;
    }
    tcflush(fd, TCIOFLUSH);
    return fd;
}

void close_serial(int fd)
{
    close(fd);
}

int write_serial(int fd, const void * data, size_t n)
{
    if (!data || n == 0) {
        return -1;
    }
    return write(fd, data, n);
}

int read_serial(int fd, void * data, size_t n, int timeout)
{
    int ret;
    fd_set readable_fd_set;
    struct timeval tout;

    if (!data || n == 0) {
        return -1;
    }
    FD_ZERO(&readable_fd_set);
    FD_SET(fd, &readable_fd_set);

    tout.tv_sec  = timeout;	/* timeout in seconds */
    tout.tv_usec = 0;

    memset(data, 0x00, n);

not_timed_out_yet:
    ret = select((fd + 1), &readable_fd_set, NULL, NULL, &tout);

    if (!ret)
        return -ETIMEOUT;

    /* hw problems... */
    if (ret < 0)
        return -EIORECV;

    ret = read(fd, data, n);

    /* hw problems... */
    if (ret < 0)
        return -EIORECV;

    /* this could not happen (we are using select to prevent this)	*/
    if (!ret) { /*  but... if this happen: WARNING !!!		*/
        if (tout.tv_usec) /* Linux dependent thing...see man select(2) */
            goto not_timed_out_yet; /* NOTES section	*/
        else
            return -ETIMEOUT;
    }
    return ret;
}


